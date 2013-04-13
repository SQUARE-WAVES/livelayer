#ifndef MIDI_PORT_DOT_H
#define MIDI_PORT_DOT_H

#include "rtmidi.h"
#include "event_loop.h"
#include "event.h"

class midi_event : public event
{
	protected:
		std::vector<unsigned char>* mess;

	public:
		midi_event(std::vector<unsigned char> *message);
		~midi_event();
		void handle();
};

class midi_port
{
	private:
		RtMidiIn inport;
		std::vector<unsigned char> message;
		bool status_registry[127];
		bool active;

		event_loop* loop;
		
		static void midi_callback(double timeStamp, std::vector<unsigned char>* message, void *userData);

		void midi_in(double timeStamp,std::vector<unsigned char>* message);
		
	public:
		midi_port();
		midi_port(event_loop* looper,int in_port);

		void ignore_type(bool sysex, bool time, bool sense);
};

#endif