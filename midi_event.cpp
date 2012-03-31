#include "SDL.h"
#include <vector>
#include "midi_event.h"

midi_event::midi_event(std::vector<uint8_t>* message)
{
	message_size = message->size();
	bytes = new uint8_t[message_size];
	for(int i=0;i<message_size;++i)
	{
		bytes[i] = message->at(i);
	}
}

midi_event::~midi_event()
{
	delete bytes;
}

uint8_t midi_event::get_status()
{
	return bytes[0];
}

uint8_t midi_event::operator[](int index)
{
	return bytes[index];
}

int midi_event::size()
{
	return message_size;
}