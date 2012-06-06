#include "event_manager.h"
#include "serial_port.h"
#include "serial_event.h"
#include <iostream>

boost::asio::io_service serial_port::io;
boost::thread serial_port::th(threadfunc);

void serial_port::threadfunc()
{
	boost::asio::io_service::work wrk(io);
	io.run();
}

serial_port::serial_port(const char* filename,lua_State* luaref):
port(io,filename),
outbuffer(OUT_BUFFER_SIZE),
L(luaref)
{
	memset(inbuff,0,IN_BUFFER_SIZE);
	port.async_read_some(boost::asio::buffer(inbuff,256),boost::bind(&serial_port::onread,this,_1,_2));
}

void serial_port::onread(const boost::system::error_code& error,std::size_t bytes_transferred)
{
	globals::evm.push_event(new serial_event((uint8_t*)inbuff,bytes_transferred,this));
	
	//and set up for the next read
	memset(inbuff,0,strlen(inbuff));
	port.async_read_some(boost::asio::buffer(inbuff,256),boost::bind(&serial_port::onread,this,_1,_2));
}

void serial_port::stop()
{	
	io.stop();
	th.join();
}

void serial_port::write(const char* buffer, int len)
{
	bool needs_kickstart = outbuffer.empty();

	for(int i=0;i<len;++i)
	{
		try
		{
			outbuffer.push_back(buffer[i]);
		}
		catch(std::exception e)
		{
			break;
		}
	}
	
	if(needs_kickstart)
	{
		boost::asio::async_write(port,boost::asio::buffer(outbuffer.linearize(),outbuffer.size()),boost::bind(&serial_port::onwrite,this,_1,_2));
	}

}

void serial_port::onwrite(const boost::system::error_code& error,std::size_t bytes_transferred)
{
	for(unsigned int i=0;i<bytes_transferred;++i)
	{
		outbuffer.pop_front();
	}
	
	//now see if there is anything else in the buffer
	if(!outbuffer.empty())
	{
		boost::asio::async_write(port,boost::asio::buffer(outbuffer.linearize(),outbuffer.size()),boost::bind(&serial_port::onwrite,this,_1,_2));
	}
}

lua_State* serial_port::get_lua()
{
	return L;
}



//-------------------------------------------------------------------------------------------------
//						_/        _/    _/    _/_/    
//					_/        _/    _/  _/    _/   
//				_/        _/    _/  _/_/_/_/    
//			_/        _/    _/  _/    _/     
//		_/_/_/_/    _/_/    _/    _/                                                                  
//-------------------------------------------------------------------------------------------------

const char* serial_port::METATABLE_NAME = "serial_port_metatable";

void serial_port::register_serialport(lua_wrapper& L)
{
	//ok got the metatable
	luaL_newmetatable(L,serial_port::METATABLE_NAME);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index"); //set a field (no stack change)
	
	lua_pushcfunction(L,collect_serialport);
	lua_setfield(L,-2,"__gc");
	
	lua_pushcfunction(L,read_event);
	lua_setfield(L,-2,"onread");
	
	lua_pushcfunction(L,lua_write);
	lua_setfield(L,-2,"write");
	
	lua_pushcfunction(L,new_serialport);
	lua_setglobal(L,"new_serial_port");	
	
}

int serial_port::new_serialport(lua_State *L)
{
	//get the filename
	const char* filename = luaL_checkstring(L,1);
	
	//make the new port	
	serial_port* port;
	
	try
	{
		port = new serial_port(filename,L);
	}
	catch(std::exception ex)
	{
		lua_pushboolean(L,false);
		lua_pushstring(L,ex.what());
		return 2;
	}
	
	//ok now push the ud
	serial_port** ud = (serial_port**)lua_newuserdata(L,sizeof(serial_port*));
	*ud = port;
	
	//now get the metatable and setup a function registry
	luaL_getmetatable(L,serial_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)*ud);
	lua_newtable(L);
	lua_settable(L,-3);
	lua_setmetatable(L,-2);

	return 1;
}

int serial_port::collect_serialport(lua_State *L)
{
	serial_port* port = *((serial_port**)luaL_checkudata(L,1,serial_port::METATABLE_NAME));
	
	//kill it's method registry
	luaL_getmetatable(L,serial_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)port);
	lua_pushnil(L);
	lua_settable(L,-3);
	
	//kill it
	delete port;
	
	return 0;
}

int serial_port::read_event(lua_State *L)
{
	//basically just set this event in the userdata
	serial_port* port =*((serial_port**)luaL_checkudata(L,1,serial_port::METATABLE_NAME));
	
	//next up a function
	if(!lua_isfunction(L,2))
	{
		lua_pushboolean(L,false);
		lua_pushstring(L,"we need a method");
		return 2;
	}
	
	//get its method registry
	luaL_getmetatable(L,serial_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)port);
	lua_gettable(L,-2);
	//ok now the registry is on the top of the stack and the function is third from the bottom
	if(!lua_istable(L,-1))
	{
		lua_pushboolean(L,false);
		lua_pushstring(L,"can't find a registry");
		return 2;
	}
	//the name of the event
	lua_pushliteral(L,"read");
	
	//the method
	lua_pushvalue(L,2);
	lua_settable(L,-3);
	lua_pushboolean(L,true);
	return 1;
}

int serial_port::lua_write(lua_State *L)
{
	//basically just set this event in the userdata
	serial_port* port =*((serial_port**)luaL_checkudata(L,1,serial_port::METATABLE_NAME));
	
	//next up a string
	const char* towrite = luaL_checkstring(L,2);
	int len = lua_objlen(L,2);
	
	port->write(towrite,len);
	
	return 0;
}