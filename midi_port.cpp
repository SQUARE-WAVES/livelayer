#include "event_loop.h"
#include "midi_port.h"
#include <iostream>

midi_event::midi_event(midi_port* origin, std::vector<unsigned char>* message):
home_port(origin)
{
	status = message->at(0);

	if(is_sysex())
	{
		mess = new std::vector<unsigned char>(*message);
	}
	else
	{
		if(message->size() != 1)
		{
			data1 = message->at(1);
			data2 = message->at(2);
		}
	}
}

void midi_event::handle(int iobytes)
{
	home_port->on_midi(this);
}

bool midi_event::is_sysex()
{
	return status == 0xf0;
}

midi_event::~midi_event()
{
	if(is_sysex())
	{
		delete mess;
	}
}

void midi_port::master_midi_callback(double timeStamp, std::vector<unsigned char> *message, void *userData)
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
	inport.setCallback(master_midi_callback,this);

	try
	{
		inport.openPort(in_port);
	}
	catch(RtError rx)
	{
		//throw a string for now
		throw "error opening port";
	}
}

void midi_port::ignore_type(bool sysex, bool time, bool sense)
{
	inport.ignoreTypes(sysex,time,sense);
}

void midi_port::midi_in(double time_stamp,std::vector<unsigned char>* message)
{
	midi_event* ev = new midi_event(this,message);
	loop->post_message(ev);
}

void midi_port::on_midi(midi_event* ev)
{
	if(ev->is_sysex())
	{
		sysex_callback(ev->mess);	
	}
	else
	{
		midi_callback(ev->status,ev->data1,ev->data2);
	}
}

void midi_port::midi_callback(unsigned char status, unsigned char data1, unsigned char data2)
{
}

void midi_port::sysex_callback(std::vector<unsigned char>* message)
{
}

int midi_port::count_midi_ports()
{
	RtMidiIn enumerator;
	return enumerator.getPortCount();
}

std::string midi_port::port_name(int port_number)
{
	RtMidiIn enumerator;
	std::string name = enumerator.getPortName(port_number);
	return name;
}