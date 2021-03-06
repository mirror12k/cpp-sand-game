


#include <iostream>
#include <cstring>



#include <SDL/SDL.h>

#include "general.hpp"

using std::cout;
using std::endl;


#define IS_PIXEL_EMPTY(X) ((X) == color_black)

#define CLEAR_PIXEL_BITS(X) ((X) & 0xfffffff8)

#define IS_PIXEL_SOLID(X) ((X) & 0x1)
#define MAKE_PIXEL_SOLID(X) ((X) | 0x1)
#define IS_PIXEL_SAND(X) ((X) & 0x2)
#define MAKE_PIXEL_SAND(X) ((X) | 0x2)
#define IS_PIXEL_LIQUID(X) ((X) & 0x4)
#define MAKE_PIXEL_LIQUID(X) ((X) | 0x4)


bool b_close_requested = false;


size_t i_screen_width = 800;
size_t i_screen_height = 600;




uint32_t color_black, color_border,
    color_void, color_source_water, color_source_oil, color_source_methane,
    color_calcium, color_sand, color_stone,
    color_water, color_water_acid, color_water_calciated, color_water_algea,
    color_oil, color_oil_burning, color_acid,
    color_methane, color_methane_burning, color_steam, color_smoke, color_fire;



uint32_t current_brush;
int brush_size = 10;

uint32_t brush_color_1, brush_color_2, brush_color_3, brush_color_4, brush_color_5, brush_color_6, brush_color_7, brush_color_8;



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
		current_brush = brush_color_1;
	if (keys[SDLK_2])
		current_brush = brush_color_2;
	if (keys[SDLK_3])
		current_brush = brush_color_3;
	if (keys[SDLK_4])
		current_brush = brush_color_4;
	if (keys[SDLK_5])
		current_brush = brush_color_5;
	if (keys[SDLK_6])
		current_brush = brush_color_6;
	if (keys[SDLK_7])
		current_brush = brush_color_7;
	if (keys[SDLK_8])
		current_brush = brush_color_8;
	if (keys[SDLK_q])
		current_brush = color_void;
	if (keys[SDLK_w])
		current_brush = color_source_water;
	if (keys[SDLK_e])
		current_brush = color_source_methane;
	if (keys[SDLK_r])
		current_brush = color_source_oil;
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


int get_pixel_weight(uint32_t color)
{
    if (color == color_sand)
        return 1000;
    else if (color == color_calcium)
        return 1500;
    else if (color == color_water || color == color_water_acid || color == color_water_calciated || color == color_water_algea)
        return 500;
    else if (color == color_oil)
        return 300;
    else if (color == color_oil_burning)
        return 250;
    else if (color == color_acid)
        return 500;
    else if (color == color_methane || color == color_methane)
        return -200;
    else if (color == color_smoke)
        return -400;
    else if (color == color_fire)
        return -200;
    else if (color == color_steam)
        return -100;
    else if (color == color_black)
        return 0;
    else
        return 0;
}


void run_sand (uint32_t* sand, uint32_t* target_buffer)
{
    memcpy(target_buffer, sand, i_screen_width * i_screen_height * 4);



    for (uint y = 1; y < i_screen_height - 1; y++)
    {
        uint32_t* top_left = target_buffer + i_screen_width * (y - 1),
            * top_center = target_buffer + 1 + i_screen_width * (y - 1),
            * top_right = target_buffer + 2 + i_screen_width * (y - 1);
        uint32_t* center_left = target_buffer + i_screen_width * y,
            * center_center = target_buffer + 1 + i_screen_width * y,
            * center_right = target_buffer + 2 + i_screen_width * y;
        uint32_t* bottom_left = target_buffer + i_screen_width * (y + 1),
            * bottom_center = target_buffer + 1 + i_screen_width * (y + 1),
            * bottom_right = target_buffer + 2 + i_screen_width * (y + 1);

        for (uint x = 1; x < i_screen_width - 1; x++)
        {
            uint32_t color = sand[x + i_screen_width * y];
            if (color != color_black && color == *center_center)
            {
                if (color == color_smoke && rand() % 100 == 0) // smoke decomposition
                {
                    *center_center = color_black;
                }
                else if (color == color_fire && rand() % 50 == 0) // fire decomposition
                {
                    *center_center = color_smoke;
                }
                else if (color == color_steam && rand() % 500 == 0) // steam cooling
                {
                    *center_center = color_water;
                }
                else if ((color == color_water || color == color_water_acid) && rand() % 2 == 0 && (
                    *top_center == color_fire ||
                    *center_left == color_fire || *center_right == color_fire ||
                    *bottom_center == color_fire
                )) // water/water_acid boiling from fire
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_fire)
                        {
                            *center_center = color_steam;
                            *top_center = color_black;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_fire)
                        {
                            *center_center = color_steam;
                            *center_left = color_black;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_fire)
                        {
                            *center_center = color_steam;
                            *center_right = color_black;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_fire)
                        {
                            *center_center = color_steam;
                            *bottom_center = color_black;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water && rand() % 2 == 0 && (
                    *top_center == color_acid ||
                    *center_left == color_acid || *center_right == color_acid ||
                    *bottom_center == color_acid
                )) // acid dissolving in water
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_acid)
                        {
                            *center_center = color_water_acid;
                            *top_center = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_acid)
                        {
                            *center_center = color_water_acid;
                            *center_left = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_acid)
                        {
                            *center_center = color_water_acid;
                            *center_right = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_acid)
                        {
                            *center_center = color_water_acid;
                            *bottom_center = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water_acid && rand() % 50 == 0 && (
                    *top_center == color_water_acid ||
                    *center_left == color_water_acid || *center_right == color_water_acid ||
                    *bottom_center == color_water_acid
                )) // acid depositing out of concentrated acidic water
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_water_acid)
                        {
                            *center_center = color_acid;
                            *top_center = color_water;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_water_acid)
                        {
                            *center_center = color_acid;
                            *center_left = color_water;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_water_acid)
                        {
                            *center_center = color_acid;
                            *center_right = color_water;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_water_acid)
                        {
                            *center_center = color_acid;
                            *bottom_center = color_water;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water_acid && rand() % 50 == 0 && (
                    *top_center == color_sand ||
                    *center_left == color_sand || *center_right == color_sand ||
                    *bottom_center == color_sand
                )) // acidic water dissolving sand to calciated water
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_sand)
                        {
                            *center_center = color_black;
                            *top_center = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_sand)
                        {
                            *center_center = color_black;
                            *center_left = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_sand)
                        {
                            *center_center = color_black;
                            *center_right = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_sand)
                        {
                            *center_center = color_black;
                            *bottom_center = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water_calciated && rand() % 400 == 0 && (
                    *top_center == color_water_calciated ||
                    *center_left == color_water_calciated || *center_right == color_water_calciated ||
                    *bottom_center == color_water_calciated
                )) // heavily calcinated water depositing calcium
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_water_calciated)
                        {
                            *center_center = color_calcium;
                            *top_center = color_water;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_water_calciated)
                        {
                            *center_center = color_calcium;
                            *center_left = color_water;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_water_calciated)
                        {
                            *center_center = color_calcium;
                            *center_right = color_water;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_water_calciated)
                        {
                            *center_center = color_calcium;
                            *bottom_center = color_water;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_calcium && rand() % 500 == 0 && (
                    *top_center == color_water ||
                    *center_left == color_water || *center_right == color_water ||
                    *bottom_center == color_water
                )) // water lightly disolving calcium
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_water)
                        {
                            *center_center = color_water_calciated;
                            *top_center = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_water)
                        {
                            *center_center = color_water_calciated;
                            *center_left = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_water)
                        {
                            *center_center = color_water_calciated;
                            *center_right = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_water)
                        {
                            *center_center = color_water_calciated;
                            *bottom_center = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water_acid && rand() % 10 == 0 && (
                    *top_center == color_calcium ||
                    *center_left == color_calcium || *center_right == color_calcium ||
                    *bottom_center == color_calcium
                )) // acidic water disolving calcium
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_calcium)
                        {
                            *center_center = color_water_calciated;
                            *top_center = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_calcium)
                        {
                            *center_center = color_water_calciated;
                            *center_left = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_calcium)
                        {
                            *center_center = color_water_calciated;
                            *center_right = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_calcium)
                        {
                            *center_center = color_water_calciated;
                            *bottom_center = color_water_calciated;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_oil && rand() % 30 == 0 && (
                    *top_center == color_fire || *top_center == color_oil_burning ||
                    *center_left == color_fire || *center_left == color_oil_burning ||
                    *center_right == color_fire || *center_right == color_oil_burning ||
                    *bottom_center == color_fire || *bottom_center == color_oil_burning
                )) // oil starting to burn
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_fire || *top_center == color_oil_burning)
                        {
                            *center_center = color_oil_burning;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_fire || *center_left == color_oil_burning)
                        {
                            *center_center = color_oil_burning;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_fire || *center_right == color_oil_burning)
                        {
                            *center_center = color_oil_burning;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_fire || *bottom_center == color_oil_burning)
                        {
                            *center_center = color_oil_burning;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_methane && rand() % 2 == 0 && (
                    *top_center == color_fire || *top_center == color_methane_burning ||
                    *center_left == color_fire || *center_left == color_methane_burning ||
                    *center_right == color_fire || *center_right == color_methane_burning ||
                    *bottom_center == color_fire || *bottom_center == color_methane_burning
                )) // methane lighting on fire
                {
                    *center_center = color_methane_burning;
                }
                else if (color == color_methane_burning && rand() % 10 == 0) // methane burning out
                {
                    *center_center = color_black;
                }
                else if (color == color_methane_burning && rand() % 5 == 0 && (
                    *top_center == color_black ||
                    *center_left == color_black ||
                    *center_right == color_black ||
                    *bottom_center == color_black
                )) // methane burning
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_black)
                        {
                            *top_center = color_fire;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_black)
                        {
                            *center_left = color_fire;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_black)
                        {
                            *center_right = color_fire;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_black)
                        {
                            *bottom_center = color_fire;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_oil_burning && rand() % 10 == 0 && (
                    *top_center == color_black ||
                    *center_left == color_black ||
                    *center_right == color_black ||
                    *bottom_center == color_black
                )) // oil burning
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_black)
                        {
                            *top_center = color_fire;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_black)
                        {
                            *center_left = color_fire;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_black)
                        {
                            *center_right = color_fire;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_black)
                        {
                            *bottom_center = color_fire;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_oil_burning && rand() % 200 == 0) // oil burning decomposition
                {
                    *center_center = color_black;
                }
                else if (color == color_water_algea && rand() % 200 == 0) // algea dying
                {
                    *center_center = color_water;
                }
                else if (color == color_water_algea && rand() % 70 == 0 && (
                    *top_center == color_water ||
                    *center_left == color_water || *center_right == color_water ||
                    *bottom_center == color_water
                )) // algea replicating
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_water)
                        {
                            *center_center = color_water_algea;
                            *top_center = color_water_algea;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_water)
                        {
                            *center_center = color_water_algea;
                            *center_left = color_water_algea;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_water)
                        {
                            *center_center = color_water_algea;
                            *center_right = color_water_algea;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_water)
                        {
                            *center_center = color_water_algea;
                            *bottom_center = color_water_algea;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water_algea && rand() % 100 == 0 && (
                    *top_center == color_water ||
                    *center_left == color_water || *center_right == color_water ||
                    *bottom_center == color_water
                )) // algea acidifying the water
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_water)
                        {
                            *top_center = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_water)
                        {
                            *center_left = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_water)
                        {
                            *center_right = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_water)
                        {
                            *bottom_center = color_water_acid;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_water_algea && rand() % 10 == 0 && (
                    *top_center == color_acid ||
                    *center_left == color_acid || *center_right == color_acid ||
                    *bottom_center == color_acid
                )) // concentrated acid killing algea
                {
                    *center_center = color_water;
                }
                else if (color == color_void) // void deleting everything
                {
                    if (*top_center != color_void && *top_center != color_border)
                    {
                        *top_center = color_black;
                    }
                    if (*center_left != color_void && *center_left != color_border)
                    {
                        *center_left = color_black;
                    }
                    if (*center_right != color_void && *center_right != color_border)
                    {
                        *center_right = color_black;
                    }
                    if (*bottom_center != color_void && *bottom_center != color_border)
                    {
                        *bottom_center = color_black;
                    }
                }
                else if (color == color_source_water && rand() % 4 == 0 && (
                    *top_center == color_black ||
                    *center_left == color_black || *center_right == color_black ||
                    *bottom_center == color_black
                )) // water source spawning
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_black)
                        {
                            *top_center = color_water;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_black)
                        {
                            *center_left = color_water;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_black)
                        {
                            *center_right = color_water;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_black)
                        {
                            *bottom_center = color_water;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_source_oil && rand() % 4 == 0 && (
                    *top_center == color_black ||
                    *center_left == color_black || *center_right == color_black ||
                    *bottom_center == color_black
                )) // oil source spawning
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_black)
                        {
                            *top_center = color_oil;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_black)
                        {
                            *center_left = color_oil;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_black)
                        {
                            *center_right = color_oil;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_black)
                        {
                            *bottom_center = color_oil;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (color == color_source_methane && rand() % 4 == 0 && (
                    *top_center == color_black ||
                    *center_left == color_black || *center_right == color_black ||
                    *bottom_center == color_black
                )) // methane source spawning
                {
                    bool not_moved = true;
                    while (not_moved)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (*top_center == color_black)
                        {
                            *top_center = color_methane;
                            not_moved = false;
                        }
                        break;
                        case 1:
                        if (*center_left == color_black)
                        {
                            *center_left = color_methane;
                            not_moved = false;
                        }
                        break;
                        case 2:
                        if (*center_right == color_black)
                        {
                            *center_right = color_methane;
                            not_moved = false;
                        }
                        break;
                        case 3:
                        if (*bottom_center == color_black)
                        {
                            *bottom_center = color_methane;
                            not_moved = false;
                        }
                        break;
                        }
                    }
                }
                else if (IS_PIXEL_SAND(color)) // sand physics
                {
                    bool left_free = ! IS_PIXEL_SOLID(*bottom_left);
                    bool center_free = ! IS_PIXEL_SOLID(*bottom_center);
                    bool right_free = ! IS_PIXEL_SOLID(*bottom_right);

                    if (left_free || center_free || right_free)
                    {
                        switch (rand() % 4)
                        {
                        case 0:
                        if (left_free)
                        {
                            *center_center = *bottom_left;
                            *bottom_left = color;
                        }
                        else
                        {
                            switch (rand() % 2)
                            {
                            case 0:
                            if (center_free)
                            {
                                *center_center = *bottom_center;
                                *bottom_center = color;
                            }
                            else
                            {
                                *center_center = *bottom_right;
                                *bottom_right = color;
                            }
                            break;
                            case 1:
                            if (right_free)
                            {
                                *center_center = *bottom_right;
                                *bottom_right = color;
                            }
                            else
                            {
                                *center_center = *bottom_center;
                                *bottom_center = color;
                            }
                            break;
                            }
                        }
                        break;

                        case 1:
                        if (center_free)
                        {
                            *center_center = *bottom_center;
                            *bottom_center = color;
                        }
                        else
                        {
                            switch (rand() % 2)
                            {
                            case 0:
                            if (left_free)
                            {
                                *center_center = *bottom_left;
                                *bottom_left = color;
                            }
                            else
                            {
                                *center_center = *bottom_right;
                                *bottom_right = color;
                            }
                            break;
                            case 1:
                            if (right_free)
                            {
                                *center_center = *bottom_right;
                                *bottom_right = color;
                            }
                            else
                            {
                                *center_center = *bottom_left;
                                *bottom_left = color;
                            }
                            break;
                            }
                        }
                        break;

                        case 2:
                        if (right_free)
                        {
                            *center_center = *bottom_right;
                            *bottom_right = color;
                        }
                        else
                        {
                            switch (rand() % 2)
                            {
                            case 0:
                            if (left_free)
                            {
                                *center_center = *bottom_left;
                                *bottom_left = color;
                            }
                            else
                            {
                                *center_center = *bottom_center;
                                *bottom_center = color;
                            }
                            break;
                            case 1:
                            if (center_free)
                            {
                                *center_center = *bottom_center;
                                *bottom_center = color;
                            }
                            else
                            {
                                *center_center = *bottom_left;
                                *bottom_left = color;
                            }
                            break;
                            }
                        }
                        break;
                        }
                    }
                }
                else if (IS_PIXEL_LIQUID(color)) // liquid physics
                {
                    int weight = get_pixel_weight(color);

                    bool top_left_free = (! IS_PIXEL_SOLID(*top_left)) &&
                        *top_left != color &&
                        (get_pixel_weight(*top_left) > weight ||
                            (get_pixel_weight(*top_left) == weight && rand() % 4 == 0)
                        );
                    bool top_center_free = (! IS_PIXEL_SOLID(*top_center)) &&
                        *top_center != color &&
                        (get_pixel_weight(*top_center) > weight ||
                            (get_pixel_weight(*top_center) == weight && rand() % 4 == 0)
                        );
                    bool top_right_free = (! IS_PIXEL_SOLID(*top_right)) &&
                        *top_right != color &&
                        (get_pixel_weight(*top_right) > weight ||
                            (get_pixel_weight(*top_right) == weight && rand() % 4 == 0)
                        );

                    bool left_free = (! IS_PIXEL_SOLID(*bottom_left)) &&
                        *bottom_left != color &&
                        (get_pixel_weight(*bottom_left) < weight ||
                            (get_pixel_weight(*bottom_left) == weight && rand() % 4 == 0)
                        );
                    bool center_free = (! IS_PIXEL_SOLID(*bottom_center)) &&
                        *bottom_center != color &&
                        (get_pixel_weight(*bottom_center) < weight ||
                            (get_pixel_weight(*bottom_center) == weight && rand() % 4 == 0)
                        );
                    bool right_free = (! IS_PIXEL_SOLID(*bottom_right)) &&
                        *bottom_right != color &&
                        (get_pixel_weight(*bottom_right) < weight ||
                            (get_pixel_weight(*bottom_right) == weight && rand() % 4 == 0)
                        );

                    bool center_left_free = (! IS_PIXEL_SOLID(*center_left)) &&
                        *center_left != color && (rand() % 3 == 0);
//                         &&
//                        (get_pixel_weight(*center_left) < weight ||
//                            (get_pixel_weight(*center_left) == weight && rand() % 4 == 0)
//                        );
                    bool center_right_free = (! IS_PIXEL_SOLID(*center_right)) &&
                        *center_right != color && (rand() % 3 == 0);
//                         &&
//                        (get_pixel_weight(*center_right) < weight ||
//                            (get_pixel_weight(*center_right) == weight && rand() % 4 == 0)
//                        );


                    if (left_free || center_free || right_free ||
                        top_left_free || top_center_free || top_right_free ||
                        center_left_free || center_right_free)
//                         && rand() % 4 == 0))
                    {
                        bool not_moved = true;
                        while (not_moved)
                        {
                            switch (rand() % 9)
                            {
                            case 0:
                            if (top_left_free)
                            {
                                *center_center = *top_left;
                                *top_left = color;
                                not_moved = false;
                            }
                            break;
                            case 1:
                            if (top_center_free)
                            {
                                *center_center = *top_center;
                                *top_center = color;
                                not_moved = false;
                            }
                            break;
                            case 2:
                            if (top_right_free)
                            {
                                *center_center = *top_right;
                                *top_right = color;
                                not_moved = false;
                            }
                            break;
                            case 3:
                            if (center_left_free)
                            {
                                *center_center = *center_left;
                                *center_left = color;
                                not_moved = false;
                            }
                            break;
                            case 4:
                            if (center_right_free)
                            {
                                *center_center = *center_right;
                                *center_right = color;
                                not_moved = false;
                            }
                            break;

                            case 5:
                            if (left_free)
                            {
                                *center_center = *bottom_left;
                                *bottom_left = color;
                                not_moved = false;
                            }
                            break;

                            case 6:
                            if (center_free)
                            {
                                *center_center = *bottom_center;
                                *bottom_center = color;
                                not_moved = false;
                            }
                            break;

                            case 7:
                            if (right_free)
                            {
                                *center_center = *bottom_right;
                                *bottom_right = color;
                                not_moved = false;
                            }
                            break;

                            case 8:
                            not_moved = false;
                            break;
                            }
                        }
                    }
                }
            }
            top_left++;
            top_center++;
            top_right++;
            center_left++;
            center_center++;
            center_right++;
            bottom_left++;
            bottom_center++;
            bottom_right++;
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
    color_border = MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 255, 255, 255)));
    color_void = MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 230, 230, 230)));

    color_source_water = MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 30, 30, 108)));
    color_source_oil = MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 40, 0, 40)));
    color_source_methane = MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 40, 138, 40)));

    color_stone = MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 128, 128, 128)));
    color_sand = MAKE_PIXEL_SAND(MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 200, 100, 50))));
    color_calcium = MAKE_PIXEL_SAND(MAKE_PIXEL_SOLID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 164, 164, 164))));

    color_water = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 50, 50, 128)));
    color_water_acid = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 140, 80, 168)));
    color_water_calciated = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 110, 150, 128)));
    color_water_algea = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 90, 40, 40)));
    color_acid = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 230, 30, 100)));

    color_oil = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 60, 20, 60)));
    color_oil_burning = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 80, 40, 60)));

    color_methane = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 90, 188, 90)));
    color_methane_burning = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 120, 188, 90)));
    color_steam = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 90, 90, 188)));
    color_smoke = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 30, 30, 30)));
    color_fire = MAKE_PIXEL_LIQUID(CLEAR_PIXEL_BITS(SDL_MapRGB(srf_backbuffer->format, 255, 128, 64)));

    current_brush = color_water;

    brush_color_1 = color_water;
    brush_color_2 = color_calcium;
    brush_color_3 = color_oil;
    brush_color_4 = color_stone;
    brush_color_5 = color_acid;
    brush_color_6 = color_fire;
    brush_color_7 = color_water_algea;
    brush_color_8 = color_methane;

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



