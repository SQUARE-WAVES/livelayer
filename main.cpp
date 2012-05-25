#include <boost/cstdint.hpp>
#include "rtmidi.h"
#include "lua_wrapper.h"
#include "midi_port.h"
#include "serial_port.h"
#include "event_manager.h"
#include <iostream>

lua_wrapper lua;
event_manager globals::evm(&lua);

int main(int argc,char* argv[])
{	
	using globals::evm;
	
	lua.register_module(midi_port::register_midiport);
	lua.register_module(serial_port::register_serialport);
	
	if(0 != lua.open_script("./scripts/testscript.lua"))
	{
		lua.print_error();
		return -1;
	}
	
	evm.event_loop();

	return 0;
}