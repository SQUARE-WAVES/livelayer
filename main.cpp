#include <stdint.h>
#include "lua_wrapper.h"
#include "event_loop.h"
#include "serial_port.h"
#include "midi_port.h"
#include <iostream>
#include <RtMidi.h>
#include <thread>

lua_wrapper lua;
event_loop loop;
//serial_port sp(&loop,"COM3");
midi_port mid(&loop,0);

/*
void funzies(event_loop& ev)
{
	char buffar[256];

	while(1)
	{
		std::cin.getline(buffar,256,'\n');
		sp.write(buffar,strlen(buffar));
	}

}*/

//a quick function to clean everything up!
void cleanup()
{
}

int main(int argc,char* argv[])
{
	/*
	if(0 != lua.open_script("scripts/testscript.lua"))
	{
		lua.print_error();
		cleanup();
		return -1;
	}
	*/

	//std::thread t1(funzies,loop);
	//t1.detach();
	
	loop.run();

	cleanup();
	return 0;
}