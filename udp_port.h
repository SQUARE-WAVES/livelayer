#ifndef UDP_PORT_DOT_H
#define UDP_PORT_DOT_H
#define UDP_BUFF_SIZE 10240

#include <stdint.h>
#include "event.h"
#include "event_loop.h"


class udp_port;

class udp_read_event : public event
{
	private:
		udp_port* home_port;
	
	public:
		udp_read_event(udp_port* port);
		void handle(int iobytes);
};

class udp_port
{
	protected:
		uint8_t buffer[UDP_BUFF_SIZE];
		SOCKET sock;
		
		event_loop* theloop;
		udp_read_event ev;

		virtual void read_callback(uint8_t* buffer, int len);

	public:
		static bool initialize();

		udp_port(unsigned int inaddr,int portnum,event_loop* looper);
		bool send_to(unsigned int addr, int port, uint8_t* buffer,int len);

		void on_read(int read_length);
};

#endif