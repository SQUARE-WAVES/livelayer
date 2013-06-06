#include <stdint.h>
#include "event.h"
#include "event_loop.h"
#include <iostream>

#include <Windows.h>

void event_loop::init_windows()
{
	iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,1);
	overlapped = NULL;
	io_bytes = 0;
	comp_key = 0;
}

event_loop::event_loop():
keep_running(true)
{
	init_windows();
}

int event_loop::wait_for_events()
{
	overlapped = NULL;
	io_bytes = 0;
	comp_key = 0;
	GetQueuedCompletionStatus(iocp_handle,&io_bytes,&comp_key,&overlapped,INFINITE);
	return io_bytes;
}

void event_loop::handle_events(int iobytes)
{
	if(comp_key != -1)
	{
		event* ev = (event*)overlapped;
		ev->handle(iobytes);
	}
	else
	{
		keep_running = false;
	}
}

void event_loop::run()
{
	keep_running = true;

	while(keep_running)
	{
		int io_bytes = wait_for_events();
		handle_events(io_bytes);
	}
}

void event_loop::post_event(event* ev,int key)
{
	PostQueuedCompletionStatus(iocp_handle,0,key,ev);
}

HANDLE event_loop::get_handle()
{
	return iocp_handle;
}

void event_loop::stop()
{
	post_event(NULL,-1);
}