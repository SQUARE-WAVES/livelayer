#include <boost/cstdint.hpp>
#include "lua_wrapper.h"
#include "midi_event.h"
#include "serial_event.h"
#include "event_manager.h"
#include <iostream>

event_manager::event_manager(lua_wrapper* l):
event_queue(1024),
keep_running(true),
mutex()
{
	L = l;
}

void event_manager::event_loop()
{
	while(keep_running)
	{
		//lock this so this function waits until events are available
		//mutex.lock();
		int len = event_queue.size();
		
		//get all the latest events and deal with them
		for(int i=0;i<len;++i)
		{
			event* ev = event_queue.front();
			ev->handle();
			event_queue.pop();
			delete ev;
		}
		
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

void event_manager::push_event(event* ev)
{
	event_queue.push(ev);	
	mutex.unlock();
}