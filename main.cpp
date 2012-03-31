#include "SDL.h"
#include "rtmidi.h"
#include "lua_wrapper.h"
#include "surface_wrapper.h"
#include "midi_port.h"
#include "serial_port.h"
#include "event_manager.h"
#include <iostream>


int main(int argc,char* argv[])
{
	lua_wrapper lua;
	event_manager evm(&lua);
	
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	atexit(SDL_Quit);
	lua.register_module(midi_port::register_midiport);
	lua.register_module(serial_port::register_serialport);
	
	//surface_wrapper screen(SDL_SetVideoMode(640,480,32,SDL_SWSURFACE));
	
	if(0 != lua.open_script("testscript.lua"))
	{
		lua.print_error();
		return -1;
	}
	
	SDL_Event event;
	
	mainloop:
  {
    // Poll for SDL events, and handle the ones we care about.

    while (SDL_PollEvent(&event)) 
    {
			evm.dispatch_event(event);
		}

		SDL_Delay(1);
	}
	goto mainloop;
	
	return 0;
}