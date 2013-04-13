#ifndef SERIAL_PORT_DOT_H
#define SERIAL_PORT_DOT_H

#include <stdint.h>
#include "lua_wrapper.h"
#include "event.h"
#include "event_loop.h"

class serial_port;

class serial_read_event : public event
{
	serial_port* home_port;

	public:
		serial_read_event(serial_port* port);
		void handle();
};

class serial_write_event : public event
{
	serial_port* home_port;

	public:
		serial_write_event(serial_port* port);
		void handle();
};


class serial_port
{
	protected:
		event_loop* loop;
		char in_buff[256];
		
		serial_read_event read_ev;
		serial_write_event write_ev;

		void start_read();

		//platform specifics---------------------------------------------------------------------------
		HANDLE port_handle;
		

	public:
		serial_port(event_loop* looper,const char* port_name);
		void write(char* buff,int size);
		void on_read();
		void on_write();
};

#endif