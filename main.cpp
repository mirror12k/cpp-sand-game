


#include <iostream>

#include <SDL/SDL.h>

#include "general.hpp"

using std::cout;
using std::endl;



bool b_close_requested = false;


void process_events ()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            b_close_requested = true;
            return;
        }
    }
}


void drawPixel(SDL_Surface* surf, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	if (SDL_MUSTLOCK(surf))
		if (SDL_LockSurface(surf) < 0)
			return;

	uint32_t c = SDL_MapRGB(surf->format, r, g, b);

	((uint32_t*)surf->pixels)[surf->pitch * y / 4 + x] = c;

	if (SDL_MUSTLOCK(surf))
		SDL_UnlockSurface(surf);
}


int main (int argc, char** argv)
{

    srand(time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        die("SDL failed to initialize");

    uint32_t i_video_options = 0;
    i_video_options |= SDL_SWSURFACE;

    SDL_Surface* srf_backbuffer = SDL_SetVideoMode(800, 600, 32, i_video_options);



    while (b_close_requested == false)
    {
        process_events();


        for (int i = 0; i < 50 * 50; i++)
            drawPixel(srf_backbuffer, rand() % 800, rand() % 600, rand() & 0xff, rand() & 0xff, rand() & 0xff);

		SDL_Flip(srf_backbuffer);
		SDL_Delay(15);
    }

	SDL_Quit();

    return 0;
}



