#ifndef MIDI_PORT_DOT_H
#define MIDI_PORT_DOT_H

#include "SDL.h"
#include <lua.hpp>
#include "lua_wrapper.h"
#include "rtmidi.h"

class midi_port
{
	private:
		RtMidiIn inport;
		std::vector<unsigned char> message;
		bool status_registry[127];
		bool active;
		
		lua_State* L;
		
	public:
		midi_port(lua_State* L);
		midi_port(int in_port, lua_State* L);
		//bool is_registered(uint8_t status);
		void ignore_type(bool sysex, bool time, bool sense);
		lua_State* get_lua();
		
		//lua shit
		static const char* METATABLE_NAME;
		static void register_midiport(lua_wrapper& L);
		
		static int register_status(lua_State *L);
		static int lua_ignore_type(lua_State *L);
		
		static int new_midi_port(lua_State *L);
		static int collect_midi_port(lua_State *L);
};

#endif