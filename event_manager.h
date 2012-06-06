#ifndef EVENT_MANAGER_DOT_H
#define EVENT_MANAGER_DOT_H

#include "midi_port.h"
#include "midi_event.h"
#include "serial_port.h"
#include "serial_event.h"

class event_manager
{
	private:
		lua_wrapper* L;
		
		boost::mutex mutex;
		Message_Queue<event*> event_queue;
		bool keep_running;
		
	public:
		event_manager(lua_wrapper* l);
		
		void push_event(event* ev);
		void event_loop();
		
		//lua stuff
		
		
};

namespace globals
{
	extern event_manager evm;
}

#endif