#include "SDL.h"
#include "lua_wrapper.h"
#include "midi_event.h"
#include "serial_event.h"
#include "event_manager.h"
#include <iostream>

event_manager::event_manager(lua_wrapper* l)
{
	L = l;
}

void event_manager::midi_event_handler(midi_event& ev,midi_port* port)
{
	//meta is on top of the stack
	luaL_getmetatable(*L,midi_port::METATABLE_NAME);
	lua_pushlightuserdata(*L,(void*)port);
	lua_gettable(*L,-2);
	if(!lua_istable(*L,-1))
	{
		std::cout<<"PANIC:no registry\n";
		return;
	}
	//so either the event registry is on top of the stack
	lua_pushnumber(*L,ev.get_status());
	lua_gettable(*L,-2);
	//so now the method or nil is on top of the stack
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	for(int i=0;i<ev.size();++i)
	{
		lua_pushnumber(*L,ev[i]);
	}
	
	lua_call(*L,ev.size(),0);
}


void event_manager::serial_event_handler(serial_event& ev, serial_port* port)
{
	//get that meta on top of the stack
	luaL_getmetatable(*L,serial_port::METATABLE_NAME);
	lua_pushlightuserdata(*L,(void*)port);
	lua_gettable(*L,-2);
	
	if(!lua_istable(*L,-1))
	{
		std::cout<<"PANIC:no registry\n";
		return;
	}
	
	//now we have the registry on top of the stack
	lua_pushliteral(*L,"read");
	lua_gettable(*L,-2);
	
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	lua_pushlstring(*L,(const char*)ev.getbytes(),ev.size());
	lua_call(*L,1,0);
	
}

void event_manager::keyup_handler(SDL_Event& ev)
{
	lua_getglobal(*L,"event_handlers_table");

	if (!lua_istable(*L, -1))
	{
		return;
	}
	
	lua_getfield(*L,-1,"keyup_handler");
	
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	lua_pushnumber(*L,ev.key.keysym.sym);
	lua_pushnumber(*L,ev.key.keysym.mod);
	lua_pushnumber(*L,ev.key.keysym.scancode);
	lua_pushnumber(*L,ev.key.keysym.unicode);
	lua_pushnumber(*L,ev.key.state);
	lua_pushnumber(*L,ev.key.type);
	lua_pushnumber(*L,ev.key.which);
	
	lua_call(*L,7,0);
}

void event_manager::keydown_handler(SDL_Event& ev)
{
	lua_getglobal(*L,"event_handlers_table");

	if (!lua_istable(*L, -1))
	{
		return;
	}
	
	lua_getfield(*L,-1,"keydown_handler");
	
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	lua_pushnumber(*L,ev.key.keysym.sym);
	lua_pushnumber(*L,ev.key.keysym.mod);
	lua_pushnumber(*L,ev.key.keysym.scancode);
	lua_pushnumber(*L,ev.key.keysym.unicode);
	lua_pushnumber(*L,ev.key.state);
	lua_pushnumber(*L,ev.key.type);
	lua_pushnumber(*L,ev.key.which);
	
	lua_call(*L,7,0);
}

void event_manager::mousemove_handler(SDL_Event& ev)
{
	lua_getglobal(*L,"event_handlers_table");

	if (!lua_istable(*L, -1))
	{
		return;
	}
	
	lua_getfield(*L,-1,"mousemove_handler");
	
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	lua_pushnumber(*L,ev.motion.x);
	lua_pushnumber(*L,ev.motion.y);
	
	lua_call(*L,2,0);
}

void event_manager::mousebuttondown_handler(SDL_Event& ev)
{
	lua_getglobal(*L,"event_handlers_table");

	if (!lua_istable(*L, -1))
	{
		return;
	}
	
	lua_getfield(*L,-1,"mousedown_handler");
	
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	lua_pushnumber(*L,ev.button.x);
	lua_pushnumber(*L,ev.button.y);
	lua_pushnumber(*L,ev.button.button);
	
	lua_call(*L,3,0);
}

void event_manager::mousebuttonup_handler(SDL_Event& ev)
{
	lua_getglobal(*L,"event_handlers_table");

	if (!lua_istable(*L, -1))
	{
		return;
	}
	
	lua_getfield(*L,-1,"mouseup_handler");
	
	if(!lua_isfunction(*L,-1))
	{
		return;
	}
	
	lua_pushnumber(*L,ev.button.x);
	lua_pushnumber(*L,ev.button.y);
	lua_pushnumber(*L,ev.button.button);
	
	lua_call(*L,3,0);
}


//=================================================================================================
void event_manager::dispatch_uevent(SDL_Event &ev)
{
	switch(ev.user.code)
	{
		case MIDI_EVENT:

			midi_event_handler(*((midi_event*)ev.user.data1),(midi_port*)ev.user.data2);
			delete ev.user.data1;
			
		break;
		
		case SERIAL_READ:
		
			serial_event_handler(*((serial_event*)ev.user.data1),(serial_port*)ev.user.data2);
			delete ev.user.data1;		
		
		break;
		default:
		break;
	}
}

void event_manager::dispatch_event(SDL_Event &ev)
{
	switch(ev.type)
	{
		case SDL_ACTIVEEVENT:
			//std::cout<<"active\n";
		break;
    case SDL_KEYDOWN:
			keydown_handler(ev);
    break;
    case SDL_KEYUP:
			keyup_handler(ev);
    break;
    case SDL_MOUSEMOTION:
			mousemove_handler(ev);
    break;
    case SDL_MOUSEBUTTONDOWN:
			mousebuttondown_handler(ev);
    break;
    case SDL_MOUSEBUTTONUP:
			mousebuttonup_handler(ev);
    break;
		case SDL_USEREVENT:
			dispatch_uevent(ev);
		break;
		case SDL_QUIT:
			exit(0);
		break;
		default:
			//std::cout<<"other sdl_event\n";
		break;
	}
}


/*-=-=-=-=-=-=-=-=-=-=-
SDL_NOEVENT = 0,       
SDL_JOYAXISMOTION,
SDL_JOYBALLMOTION,
SDL_JOYHATMOTION,
SDL_JOYBUTTONDOWN,
SDL_JOYBUTTONUP,
SDL_SYSWMEVENT,
SDL_EVENT_RESERVEDA,
SDL_EVENT_RESERVEDB,
SDL_VIDEORESIZE,
SDL_VIDEOEXPOSE,
SDL_EVENT_RESERVED2,
SDL_EVENT_RESERVED3,
SDL_EVENT_RESERVED4,
SDL_EVENT_RESERVED5,
SDL_EVENT_RESERVED6,
SDL_EVENT_RESERVED7,      
SDL_NUMEVENTS = 32
-=-=-=-=-=-=-=-=-=-=-*/