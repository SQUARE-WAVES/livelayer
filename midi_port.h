#ifndef MIDI_PORT_DOT_H
#define MIDI_PORT_DOT_H

#include "rtmidi.h"
#include "event_loop.h"
#include "event.h"

class midi_port;

class midi_event : public event
{
	public:
		unsigned char status;
		unsigned char data1;
		unsigned char data2;

		std::vector<unsigned char> *mess;

		bool is_sysex();

		midi_port* home_port;

		midi_event(midi_port* origin, std::vector<unsigned char> *message);
		~midi_event();
		void handle(int iobytes);
};

class midi_port
{
	protected:
		RtMidiIn inport;
		bool active;

		event_loop* loop;
		
		static void master_midi_callback(double timeStamp, std::vector<unsigned char>* message, void *userData);
		void midi_in(double timeStamp,std::vector<unsigned char>* message);

		virtual void midi_callback (unsigned char status, unsigned char data1, unsigned char data2);
		virtual void sysex_callback (std::vector<unsigned char>* message);

	public:
		static int count_midi_ports();
		static std::string port_name(int port_number);

		midi_port();
		midi_port(event_loop* looper,int in_port);

		void ignore_type(bool sysex, bool time, bool sense);
		void on_midi(midi_event* ev);

};

#endif