#ifndef EVENT_DOT_H
#define EVENT_DOT_H

#include <Windows.h>

class event : public OVERLAPPED
{
	protected:
		
	public:
		event();

		virtual void handle(int iobytes) = 0; //executes the event
		virtual ~event() {} //no pure virtual destructors!
};

#endif