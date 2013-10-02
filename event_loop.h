#ifndef EVENT_LOOP_DOT_H
#define EVENT_LOOP_DOT_H
#include "event.h"

class event_loop
{
	private:
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

		void post_event(event* ev);
		void post_message(event* ev);

		void stop();
		void run();

		static const int EXIT_KEY = -1;
		static const int EVENT_KEY = 1;
		static const int MESSAGE_KEY = 3;
};

#endif