#ifndef CLUA_DOT_H
#define CLUA_DOT_H

#include <lua.hpp>

class lua_wrapper
{
	private:
		lua_State* L;

	public:
		lua_wrapper();
		~lua_wrapper();
		
		inline operator lua_State* (){return L;}
		
		int open_script(const char* path);
		void print_error();
		void register_function(const char* name, int (*method) (lua_State* Lwrap));
		void register_module(void (*method)(lua_wrapper& L));
		
};

#endif