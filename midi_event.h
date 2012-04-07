#ifndef CMIDI_EVENT_DOT_H
#define CMIDI_EVENT_DOT_H

#include "midi_port.h"
#include "event.h"
#include <vector>

using boost::uint8_t;

class midi_event : public event
{
	private:
		int message_size;
		uint8_t* bytes;
		midi_port* port;
	
	public:
		midi_event(std::vector<uint8_t>* message,midi_port* port);
		~midi_event();
		
		void handle();

		int size();
		uint8_t get_status();
		uint8_t operator[](int index);
};
#endif