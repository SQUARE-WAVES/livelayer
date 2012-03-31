#ifndef EVENT_MANAGER_DOT_H
#define EVENT_MANAGER_DOT_H

#include "midi_port.h"
#include "serial_port.h"
#include "serial_event.h"

class event
{
	public:
		virtual int id() = 0;
		virtual void* data(int data_id) =0;
};

class event_manager
{
	private:
		lua_wrapper* L;
		void dispatch_uevent(SDL_Event &ev);
		void midi_event_handler(midi_event& ev,midi_port* port);
		void serial_event_handler(serial_event& ev, serial_port* port);
		void keyup_handler(SDL_Event& ev);
		void keydown_handler(SDL_Event& ev);
		void mousemove_handler(SDL_Event& ev);
		void mousebuttondown_handler(SDL_Event& ev);
		void mousebuttonup_handler(SDL_Event& ev);
		
	public:
	
		static const int MIDI_EVENT = 1;
		static const int SERIAL_READ = 2;
		event_manager(lua_wrapper* l);
		
		void dispatch_event(SDL_Event& ev);
		
};

#endif