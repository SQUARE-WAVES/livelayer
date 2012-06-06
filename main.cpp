#include <boost/cstdint.hpp>
#include "rtmidi.h"
#include "lua_wrapper.h"
#include "midi_port.h"
#include "serial_port.h"
#include "event_manager.h"
#include <iostream>

lua_wrapper lua;
event_manager globals::evm(&lua);

//a quick function to clean everything up!
void cleanup()
{
	//ok here is the cleanup
	//close lua (will this collect all the garbage?)
	//stop the serial ports.
	serial_port::stop();
	//get out!
}

int main(int argc,char* argv[])
{	
	using globals::evm;
	
	lua.register_module(midi_port::register_midiport);
	lua.register_module(serial_port::register_serialport);
	
	if(0 != lua.open_script("scripts/testscript.lua"))
	{
		lua.print_error();
		cleanup();
		return -1;
	}
	
	evm.event_loop();
	
	cleanup();
	return 0;
}