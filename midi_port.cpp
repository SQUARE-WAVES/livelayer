#include "midi_port.h"
#include "midi_event.h"

void midi_callback(double deltatime, std::vector< unsigned char > *message, void *userData)
{
	SDL_Event event;

	event.type = SDL_USEREVENT;
	event.user.code = 1;
	event.user.data1 = (void*)new midi_event(message,(midi_port*)userData);
	event.user.data2 = userData;
	
	//THIS COPIES THE EVENT
	SDL_PushEvent(&event);
	
	
}

midi_port::midi_port(lua_State* luaref):
inport(),
L(luaref)
{

	for(int i=0;i<127;++i)
	{
		status_registry[i] = false;
	}
	//for now
	inport.ignoreTypes(true,false,true);
  inport.openPort(8);
	inport.setCallback(midi_callback,this);
}

midi_port::midi_port(int in_port, lua_State* luaref):
inport(),
L(luaref)
{

	for(int i=0;i<127;++i)
	{
		status_registry[i] = false;
	}

	//for now
	inport.ignoreTypes(true,true,true);
	inport.openPort(in_port);
	inport.setCallback(midi_callback,this);
}

void midi_port::ignore_type(bool sysex, bool time, bool sense)
{
	inport.ignoreTypes(sysex,time,sense);
}

lua_State* midi_port::get_lua()
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

const char* midi_port::METATABLE_NAME = "midi_port_metatable";

void midi_port::register_midiport(lua_wrapper& L)
{
	//ok got the metatable
	luaL_newmetatable(L,midi_port::METATABLE_NAME);
	lua_pushvalue(L,-1);
	lua_setfield(L,-2,"__index"); //set a field (no stack change)
	
	lua_pushcfunction(L,collect_midi_port);
	lua_setfield(L,-2,"__gc");
	
	lua_pushcfunction(L,register_status);
	lua_setfield(L,-2,"MIDIin");
	
	lua_pushcfunction(L,lua_ignore_type);
	lua_setfield(L,-2,"ignore_types");
	
	lua_pushcfunction(L,new_midi_port);
	lua_setglobal(L,"new_midi_port");
	
}


int midi_port::collect_midi_port(lua_State *L)
{
	midi_port* theport = *((midi_port**)luaL_checkudata(L,1,midi_port::METATABLE_NAME));
	
	//kill it's method registry
	luaL_getmetatable(L,midi_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)theport);
	lua_pushnil(L);
	lua_settable(L,-3);
	
	//kill the port
	delete theport;
	
	return 0;
}

int midi_port::register_status(lua_State *L)
{
	midi_port* theport = *((midi_port**)luaL_checkudata(L,1,midi_port::METATABLE_NAME));
	if(!lua_isnumber(L,2))
	{
		lua_pushboolean(L,false);
		lua_pushstring(L,"we need a status");
		return 2;
	}
	
	if(!lua_isfunction(L,3))
	{
		lua_pushboolean(L,false);
		lua_pushstring(L,"we need a method");
		return 2;
	}
	//get its method registry
	luaL_getmetatable(L,midi_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)theport);
	lua_gettable(L,-2);
	//ok now the registry is on the top of the stack and the function is third from the bottom
	if(!lua_istable(L,-1))
	{
		lua_pushboolean(L,false);
		lua_pushstring(L,"can't find a registry");
		return 2;
	}
	//the status
	lua_pushvalue(L,2);
	//the method
	lua_pushvalue(L,3);
	lua_settable(L,-3);
	lua_pushboolean(L,true);
	return 1;
}

int midi_port::lua_ignore_type(lua_State *L)
{
	midi_port* theport = *((midi_port**)luaL_checkudata(L,1,midi_port::METATABLE_NAME));
	bool sysex = true;
	bool time = true;
	bool sense = true;
	
	if(lua_isboolean(L,2))
	{
		sysex = lua_toboolean(L,2);
	}
	
	if(lua_isboolean(L,3))
	{
		time= lua_toboolean(L,3);
	}
	
	if(lua_isboolean(L,4))
	{
		sense = lua_toboolean(L,4);
	}
	
	theport->ignore_type(sysex,time,sense);
	
	return 0;
}


int midi_port::new_midi_port(lua_State *L)
{	
	//get the number
	int portnum = luaL_checkint(L,1);
	//make the new port	
	midi_port* port;
	try
	{
		port = new midi_port(portnum,L);
	}
	catch(RtError rte)
	{
		std::cout << rte.getMessageString();
		lua_pushboolean(L,false);
		lua_pushstring(L,rte.getMessageString());
		return 2;
	}
	
	//ok now push the ud
	midi_port** ud = (midi_port**)lua_newuserdata(L,sizeof(midi_port*));
	*ud = port;
	
	//now get the metatable and setup a function registry
	luaL_getmetatable(L,midi_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)*ud);
	lua_newtable(L);
	lua_settable(L,-3);
	
	lua_setmetatable(L,-2);
	return 1;
}