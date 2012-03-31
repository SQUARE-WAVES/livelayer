#ifndef CMIDI_EVENT_DOT_H
#define CMIDI_EVENT_DOT_H

#include <vector>

class midi_event 
{
	private:
		int message_size;
		uint8_t* bytes;		
	
	public:
		midi_event(std::vector<uint8_t>* message);
		~midi_event();

		int size();
		uint8_t get_status();
		uint8_t operator[](int index);
};
#endif