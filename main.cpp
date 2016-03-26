


#include <iostream>
#include <cstring>



#include <SDL/SDL.h>

#include "general.hpp"

using std::cout;
using std::endl;



bool b_close_requested = false;


size_t i_screen_width = 800;
size_t i_screen_height = 600;




uint32_t color_black, color_border, color_sand, color_water;



uint32_t current_brush;
size_t brush_size = 10;



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




void process_input(uint32_t* sand)
{

    int x, y;
    SDL_GetMouseState(&x, &y);

    uint8_t mouse_state = SDL_GetMouseState(NULL, NULL);

    if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        for (int p_x = x - brush_size; p_x <= x + brush_size; p_x++)
            for (int p_y = y - brush_size; p_y <= y + brush_size; p_y++)
                if (p_x > 0 && p_x < i_screen_width - 1 && p_y > 0 && p_y < i_screen_height - 1)
                    sand[p_y * i_screen_width + p_x] = current_brush;
    }

    if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        for (int p_x = x - brush_size; p_x <= x + brush_size; p_x++)
            for (int p_y = y - brush_size; p_y <= y + brush_size; p_y++)
                if (p_x > 0 && p_x < i_screen_width - 1 && p_y > 0 && p_y < i_screen_height - 1)
                    sand[p_y * i_screen_width + p_x] = color_black;
    }


	uint8_t* keys = SDL_GetKeyState(NULL);
	if (keys[SDLK_ESCAPE])
		b_close_requested = true;
	if (keys[SDLK_1])
		current_brush = color_water;
	if (keys[SDLK_2])
		current_brush = color_sand;
	if (keys[SDLK_p])
        if (++brush_size > 100)
            brush_size = 100;
	if (keys[SDLK_o])
        if (--brush_size < 1)
            brush_size = 1;
}





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
    memcpy(target_buffer, sand, i_screen_width * i_screen_height * 4);

    for (int y = 1; y < i_screen_height - 1; y++)
    {
        for (int x = 1; x < i_screen_width - 1; x++)
        {
//            if (rand() % 10000 == 0)
//            {
//                target_buffer[y * i_screen_width + x] = color_sand;
//            }

            uint32_t color = sand[y * i_screen_width + x];
            if (color == color_sand)
            {
                bool left_free = target_buffer[(y + 1) * i_screen_width + x - 1] == color_black ||
                    target_buffer[(y + 1) * i_screen_width + x - 1] == color_water;
                bool center_free = target_buffer[(y + 1) * i_screen_width + x] == color_black ||
                    target_buffer[(y + 1) * i_screen_width + x] == color_water;
                bool right_free = target_buffer[(y + 1) * i_screen_width + x + 1] == color_black ||
                    target_buffer[(y + 1) * i_screen_width + x + 1] == color_water;

                if (left_free || center_free || right_free)
                {
                    switch (rand() % 4)
                    {
                    case 0:
                    if (left_free)
                    {
                        target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x - 1];
                        target_buffer[(y + 1) * i_screen_width + x - 1] = color;
                    }
                    else
                    {
                        switch (rand() % 2)
                        {
                        case 0:
                        if (center_free)
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x];
                            target_buffer[(y + 1) * i_screen_width + x] = color;
                        }
                        else
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x + 1];
                            target_buffer[(y + 1) * i_screen_width + x + 1] = color;
                        }
                        break;
                        case 1:
                        if (right_free)
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x + 1];
                            target_buffer[(y + 1) * i_screen_width + x + 1] = color;
                        }
                        else
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x];
                            target_buffer[(y + 1) * i_screen_width + x] = color;
                        }
                        break;
                        }
                    }
                    break;

                    case 1:
                    if (center_free)
                    {
                        target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x];
                        target_buffer[(y + 1) * i_screen_width + x] = color;
                    }
                    else
                    {
                        switch (rand() % 2)
                        {
                        case 0:
                        if (left_free)
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x - 1];
                            target_buffer[(y + 1) * i_screen_width + x - 1] = color;
                        }
                        else
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x + 1];
                            target_buffer[(y + 1) * i_screen_width + x + 1] = color;
                        }
                        break;
                        case 1:
                        if (right_free)
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x + 1];
                            target_buffer[(y + 1) * i_screen_width + x + 1] = color;
                        }
                        else
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x - 1];
                            target_buffer[(y + 1) * i_screen_width + x - 1] = color;
                        }
                        break;
                        }
                    }
                    break;

                    case 2:
                    if (right_free)
                    {
                        target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x + 1];
                        target_buffer[(y + 1) * i_screen_width + x + 1] = color;
                    }
                    else
                    {
                        switch (rand() % 2)
                        {
                        case 0:
                        if (left_free)
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x - 1];
                            target_buffer[(y + 1) * i_screen_width + x - 1] = color;
                        }
                        else
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x];
                            target_buffer[(y + 1) * i_screen_width + x] = color;
                        }
                        break;
                        case 1:
                        if (center_free)
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x];
                            target_buffer[(y + 1) * i_screen_width + x] = color;
                        }
                        else
                        {
                            target_buffer[y * i_screen_width + x] = target_buffer[(y + 1) * i_screen_width + x - 1];
                            target_buffer[(y + 1) * i_screen_width + x - 1] = color;
                        }
                        break;
                        }
                    }
                    break;
                    }
                }
            }
            else if (color == color_water)
            {

                bool left_free = target_buffer[(y + 1) * i_screen_width + x - 1] == color_black;
                bool center_free = target_buffer[(y + 1) * i_screen_width + x] == color_black;
                bool right_free = target_buffer[(y + 1) * i_screen_width + x + 1] == color_black;
                bool center_left_free = target_buffer[y * i_screen_width + x - 1] == color_black;
                bool center_right_free = target_buffer[y * i_screen_width + x + 1] == color_black;
                if (left_free || center_free || right_free || center_left_free || center_right_free)
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 8)
                        {
                        case 0:
                        case 1:
                        if (center_left_free)
                        {
                            target_buffer[y * i_screen_width + x] = color_black;
                            target_buffer[y * i_screen_width + x - 1] = color;
                            not_moved = false;
                        }
                        break;

                        case 2:
                        case 3:
                        if (center_right_free)
                        {
                            target_buffer[y * i_screen_width + x] = color_black;
                            target_buffer[y * i_screen_width + x + 1] = color;
                            not_moved = false;
                        }
                        break;

                        case 4:
                        if (left_free)
                        {
                            target_buffer[y * i_screen_width + x] = color_black;
                            target_buffer[(y + 1) * i_screen_width + x - 1] = color;
                            not_moved = false;
                        }
                        break;

                        case 5:
                        if (center_free)
                        {
                            target_buffer[y * i_screen_width + x] = color_black;
                            target_buffer[(y + 1) * i_screen_width + x] = color;
                            not_moved = false;
                        }
                        break;

                        case 6:
                        if (right_free)
                        {
                            target_buffer[y * i_screen_width + x] = color_black;
                            target_buffer[(y + 1) * i_screen_width + x + 1] = color;
                            not_moved = false;
                        }
                        break;

                        case 7:
                        not_moved = false;
                        break;
                        }
                    }
                }
            }
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

    SDL_Surface* srf_backbuffer = SDL_SetVideoMode(i_screen_width, i_screen_height, 32, i_video_options);

    color_black = SDL_MapRGB(srf_backbuffer->format, 0, 0, 0);
    color_border = SDL_MapRGB(srf_backbuffer->format, 255, 255, 255);
    color_sand = SDL_MapRGB(srf_backbuffer->format, 200, 100, 50);
    color_water = SDL_MapRGB(srf_backbuffer->format, 50, 50, 128);

    current_brush = color_sand;

    uint32_t* bitmap = (uint32_t*)malloc(i_screen_width * i_screen_height * 4);
    uint32_t* swap_bitmap = (uint32_t*)malloc(i_screen_width * i_screen_height * 4);

    for (uint y = 0; y < i_screen_height; y++)
        for (uint x = 0; x < i_screen_width; x++)
            bitmap[y * i_screen_width + x] = color_black;


    for (uint y = 0; y < i_screen_height; y++)
    {
        bitmap[y * i_screen_width] = color_border;
        bitmap[y * i_screen_width + i_screen_width - 1] = color_border;
    }
    for (uint x = 0; x < i_screen_width; x++)
    {
        bitmap[x] = color_border;
        bitmap[(i_screen_height - 1) * i_screen_width + x] = color_border;
    }

    int i = 0;

    while (b_close_requested == false)
    {
        process_events();

        process_input(bitmap);

        run_sand(bitmap, swap_bitmap);
        // swap the buffers
        uint32_t* swap_val = bitmap;
        bitmap = swap_bitmap;
        swap_bitmap = swap_val;

//        cout << "frame" << endl;
        // draw the sand buffer on screen
        draw_bitmap(srf_backbuffer, i_screen_width, i_screen_height, bitmap);

		SDL_Delay(15);
		SDL_Flip(srf_backbuffer);
    }

	SDL_Quit();

    return 0;
}



