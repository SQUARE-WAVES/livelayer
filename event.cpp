#include "event.h"


//sets up the OVERLAPPED structure that it depends on.
event::event()
{
		Internal = 0;
		InternalHigh = 0;
		Offset = 0;
		OffsetHigh = 0;
		Pointer = 0;
		hEvent = 0;
}