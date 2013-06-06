#ifndef EVENT_LOOP_DOT_H
#define EVENT_LOOP_DOT_H
#include "event.h"

class event_loop
{
	private:
		static const int exit_key = -1;
		bool keep_running;

		int wait_for_events();
		void handle_events(int iobytes);

		//----------------PLATFORM SPECIFICS-------------------------------------------------------------
		HANDLE iocp_handle;
		LPOVERLAPPED overlapped;
		DWORD io_bytes;
		DWORD comp_key;

		void init_windows();
		
	public:
		event_loop();

		HANDLE get_handle();

		void post_event(event* ev,int key);
		void stop();
		void run();
};

#endif