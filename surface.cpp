#include"SDL.h"
#include "surface_wrapper.h"

surface_wrapper::surface_wrapper(SDL_Surface* surf)
{
	surface = surf;
}

surface_wrapper::~surface_wrapper()
{
	SDL_FreeSurface(surface);
}

void surface_wrapper::blank(uint8_t red,uint8_t green, uint8_t blue)
{
	
}