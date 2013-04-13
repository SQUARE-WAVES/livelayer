#include "event_loop.h"
#include "midi_port.h"
#include <iostream>

midi_event::midi_event(std::vector<unsigned char>* message)
{
	mess = new std::vector<unsigned char>(*message);
}
void midi_event::handle()
{
	std::cout <<"MIDI IN\n";
	std::cout <<"status : " <<std::hex << (unsigned int)mess->at(0) <<'\n';
	delete this;
}

midi_event::~midi_event()
{
	delete mess;
}

void midi_port::midi_callback(double timeStamp, std::vector<unsigned char> *message, void *userData)
{
	midi_port* port = (midi_port*)userData;
	port->midi_in(timeStamp,message);
}

midi_port::midi_port(event_loop* looper, int in_port):
inport(),
loop(looper)
{
	//for now
	inport.ignoreTypes(true,true,true);
	inport.openPort(in_port);
	inport.setCallback(midi_callback,this);
}

void midi_port::ignore_type(bool sysex, bool time, bool sense)
{
	inport.ignoreTypes(sysex,time,sense);
}

void midi_port::midi_in(double time_stamp,std::vector<unsigned char>* message)
{
	midi_event* ev = new midi_event(message);
	loop->post_event(ev,1);
}