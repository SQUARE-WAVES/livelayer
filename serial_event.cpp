#include <boost/cstdint.hpp>
#include "serial_event.h"

serial_event::serial_event(uint8_t* message,int len,serial_port* portref):
len(len),
port(portref)
{
	this->len = len;
	bytes = new uint8_t[len];
	
	for(int i=0;i<len;++i)
	{
		bytes[i] = message[i];
	}
	
}

serial_event::~serial_event()
{
	delete[] bytes;
}

int serial_event::size()
{
	return len;
}

uint8_t* serial_event::getbytes()
{
	return bytes;
}

void serial_event::handle()
{
	lua_State* L = port->get_lua();
	
	int stacktop = lua_gettop(L);
	luaL_getmetatable(L,serial_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)port);
	lua_gettable(L,-2);
	
	if(!lua_istable(L,-1))
	{
		std::cout<<"PANIC:no registry\n";
		lua_settop(L,stacktop);
		return;
	}
	
	//now we have the registry on top of the stack
	lua_pushliteral(L,"read");
	lua_gettable(L,-2);
	
	if(!lua_isfunction(L,-1))
	{
		lua_settop(L,stacktop);
		return;
	}
	
	lua_pushlstring(L,(const char*)bytes,len);
	lua_call(L,1,0);
}