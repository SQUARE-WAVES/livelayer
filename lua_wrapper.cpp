#include "lua_wrapper.h"
#include <iostream>

lua_wrapper::lua_wrapper()
{
	L = lua_open();
	luaL_openlibs(L);
}

lua_wrapper::~lua_wrapper()
{
	lua_close(L);
}

int lua_wrapper::open_script(const char* path)
{
	return luaL_dofile(L,path);
}

void lua_wrapper::print_error()
{
	std::cout << lua_tostring(L,-1);	
}

void lua_wrapper::register_function(const char* name, int (*method) (lua_State* L))
{
	lua_pushcfunction(L, method);
	lua_setglobal(L, name);
}

void lua_wrapper::register_module(void (*method)(lua_wrapper& Lwrap))
{
	method(*this);
}