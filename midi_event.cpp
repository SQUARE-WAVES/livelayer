#include <boost/cstdint.hpp>
#include <vector>
#include "midi_event.h"

midi_event::midi_event(std::vector<uint8_t>* message,midi_port* portref)
{
	port = portref;
	message_size = message->size();
	bytes = new uint8_t[message_size];
	
	for(int i=0;i<message_size;++i)
	{
		bytes[i] = message->at(i);
	}
}

midi_event::~midi_event()
{
	delete[] bytes;
}

uint8_t midi_event::get_status()
{
	return bytes[0];
}

uint8_t midi_event::operator[](int index)
{
	return bytes[index];
}

int midi_event::size()
{
	return message_size;
}

void midi_event::handle()
{
	lua_State* L = this->port->get_lua();
	
	//save the stack!
	int stacktop = lua_gettop(L);
	
	//meta is on top of the stack
	luaL_getmetatable(L,midi_port::METATABLE_NAME);
	lua_pushlightuserdata(L,(void*)port);
	lua_gettable(L,-2);
	
	//if there is no event registry something is fucked up
	if(!lua_istable(L,-1))
	{
	 	std::cout<<"EVENT PANIC!:no registry\n";
	 	//restore the stack
		lua_settop(L,stacktop);
		return;
	}
	
	//so the event registry is on top of the stack get the handler for the status
	lua_pushnumber(L,bytes[0]);
	lua_gettable(L,-2);
	
	//so now the method or nil is on top of the stack
	if(!lua_isfunction(L,-1))
	{
		//no function, just restore the stack.
		lua_settop(L,stacktop);
		return;
	}

	//stack the shit up and call the function!	
	for(int i=0;i<message_size;++i)
	{
		lua_pushnumber(L,bytes[i]);
	}
	
	lua_call(L,message_size,0);

}