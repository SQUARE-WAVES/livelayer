#include "SDL.h"
#include "serial_event.h"

serial_event::serial_event(uint8_t* message,int len)
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
	delete bytes;
}

int serial_event::size()
{
	return len;
}

uint8_t* serial_event::getbytes()
{
	return bytes;
}