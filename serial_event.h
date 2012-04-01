#ifndef SERIAL_EVENT_DOT_H
#define SERIAL_EVENT_DOT_H
#include "event.h"
#include "serial_port.h"

class serial_event
{
	protected:
		uint8_t* bytes;
		int len;
		serial_port* port;
		
	public:
		serial_event(uint8_t* message,int len,serial_port* portref);
		~serial_event();
		
		void handle();

		int size();
		uint8_t* getbytes();
};


#endif