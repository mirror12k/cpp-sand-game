


#include <iostream>
#include <cstring>



#include <SDL/SDL.h>

#include "general.hpp"

using std::cout;
using std::endl;



bool b_close_requested = false;




void draw_bitmap(SDL_Surface* surf, int max_x, int max_y, uint32_t* bitmap)
{
	if (SDL_MUSTLOCK(surf))
		if (SDL_LockSurface(surf) < 0)
			return;
    uint32_t* surface_pixels = (uint32_t*)surf->pixels;
    for (int y = 0; y < max_y; y ++)
        memcpy(surface_pixels + surf->pitch * y / 4, bitmap + y * max_x, max_x * 4);

	if (SDL_MUSTLOCK(surf))
		SDL_UnlockSurface(surf);
}





uint32_t color_border, color_sand, color_black;




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


void run_sand (uint32_t* sand, uint32_t* target_buffer)
{
    memcpy(target_buffer, sand, 800 * 600 * 4);

    for (int y = 1; y < 600 - 1; y++)
    {
        for (int x = 1; x < 800 - 1; x++)
        {
            if (rand() % 10000 == 0)
            {
                target_buffer[y * 800 + x] = color_sand;
            }

            uint32_t color = sand[y * 800 + x];
            if (color != color_black)
            {
                bool left_free = target_buffer[(y + 1) * 800 + x - 1] == color_black;
                bool center_free = target_buffer[(y + 1) * 800 + x] == color_black;
                bool right_free = target_buffer[(y + 1) * 800 + x + 1] == color_black;


                if (left_free || center_free || right_free)
                {
//                    target_buffer[y * 800 + x] = color_black;
//                    target_buffer[(y + 1) * 800 + x] = color;
                    switch (rand() % 3)
                    {
                    case 0:
                    if (left_free)
                    {
                        target_buffer[y * 800 + x] = color_black;
                        target_buffer[(y + 1) * 800 + x - 1] = color;
                    }
                    else
                    {
                        switch (rand() % 2)
                        {
                        case 0:
                        if (center_free)
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x] = color;
                        }
                        else
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x + 1] = color;
                        }
                        break;
                        case 1:
                        if (right_free)
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x + 1] = color;
                        }
                        else
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x] = color;
                        }
                        break;
                        }
                    }
                    break;

                    case 1:
                    if (center_free)
                    {
                        target_buffer[y * 800 + x] = color_black;
                        target_buffer[(y + 1) * 800 + x] = color;
                    }
                    else
                    {
                        switch (rand() % 2)
                        {
                        case 0:
                        if (left_free)
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x - 1] = color;
                        }
                        else
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x + 1] = color;
                        }
                        break;
                        case 1:
                        if (right_free)
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x + 1] = color;
                        }
                        else
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x - 1] = color;
                        }
                        break;
                        }
                    }
                    break;

                    case 2:
                    if (right_free)
                    {
                        target_buffer[y * 800 + x] = color_black;
                        target_buffer[(y + 1) * 800 + x + 1] = color;
                    }
                    else
                    {
                        switch (rand() % 2)
                        {
                        case 0:
                        if (left_free)
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x - 1] = color;
                        }
                        else
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x] = color;
                        }
                        break;
                        case 1:
                        if (center_free)
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x] = color;
                        }
                        else
                        {
                            target_buffer[y * 800 + x] = color_black;
                            target_buffer[(y + 1) * 800 + x - 1] = color;
                        }
                        break;
                        }
                    }
                    break;
                    }
                }
            }
//            if (sand[y * 800 + x] != color_black && sand[y * 800 + x] != color_border &&
//                (left_free || center_free || right_free))
//            {
//                uint32_t color = sand[y * 800 + x];
//
//                switch (rand() % 3)
//                {
//                case 0:
//                if (left_free)
//                {
//                    sand[y * 800 + x] = color_black;
//                    sand[(y + 1) * 800 + x - 1] = color;
//                }
//                else
//                {
//                    switch (rand() % 2)
//                    {
//                    case 0:
//                    if (center_free)
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x] = color;
//                    }
//                    else
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x + 1] = color;
//                    }
//                    break;
//                    case 1:
//                    if (right_free)
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x + 1] = color;
//                    }
//                    else
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x] = color;
//                    }
//                    break;
//                    }
//                }
//                break;
//
//                case 1:
//                if (center_free)
//                {
//                    sand[y * 800 + x] = color_black;
//                    sand[(y + 1) * 800 + x] = color;
//                }
//                else
//                {
//                    switch (rand() % 2)
//                    {
//                    case 0:
//                    if (left_free)
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x - 1] = color;
//                    }
//                    else
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x + 1] = color;
//                    }
//                    break;
//                    case 1:
//                    if (right_free)
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x + 1] = color;
//                    }
//                    else
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x - 1] = color;
//                    }
//                    break;
//                    }
//                }
//                break;
//
//                case 2:
//                if (right_free)
//                {
//                    sand[y * 800 + x] = color_black;
//                    sand[(y + 1) * 800 + x + 1] = color;
//                }
//                else
//                {
//                    switch (rand() % 2)
//                    {
//                    case 0:
//                    if (left_free)
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x - 1] = color;
//                    }
//                    else
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x] = color;
//                    }
//                    break;
//                    case 1:
//                    if (center_free)
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x] = color;
//                    }
//                    else
//                    {
//                        sand[y * 800 + x] = color_black;
//                        sand[(y + 1) * 800 + x - 1] = color;
//                    }
//                    break;
//                    }
//                }
//                break;
//                }
//            }
        }
    }
}


int main (int argc, char** argv)
{

    srand(time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        die("SDL failed to initialize");

    uint32_t i_video_options = 0;
    i_video_options |= SDL_SWSURFACE;

    SDL_Surface* srf_backbuffer = SDL_SetVideoMode(800, 600, 32, i_video_options);

    color_black = SDL_MapRGB(srf_backbuffer->format, 0, 0, 0);
    color_border = SDL_MapRGB(srf_backbuffer->format, 255, 255, 255);
    color_sand = SDL_MapRGB(srf_backbuffer->format, 230, 200, 180);

    uint32_t* bitmap = (uint32_t*)malloc(800 * 600 * 4);
    uint32_t* swap_bitmap = (uint32_t*)malloc(800 * 600 * 4);

    for (uint y = 0; y < 600; y++)
        for (uint x = 0; x < 800; x++)
            bitmap[y * 800 + x] = color_black;


    for (uint y = 0; y < 600; y++)
    {
        bitmap[y * 800] = color_border;
        bitmap[y * 800 + 800 - 1] = color_border;
    }
    for (uint x = 0; x < 800; x++)
    {
        bitmap[x] = color_border;
        bitmap[(600 - 1) * 800 + x] = color_border;
    }

    int i = 0;

    while (b_close_requested == false)
    {
        process_events();

        run_sand(bitmap, swap_bitmap);
        uint32_t* swap_val = bitmap;
        bitmap = swap_bitmap;
        swap_bitmap = swap_val;

        cout << "frame" << endl;
        draw_bitmap(srf_backbuffer, 800, 600, bitmap);

		SDL_Flip(srf_backbuffer);
		SDL_Delay(15);
    }

	SDL_Quit();

    return 0;
}



