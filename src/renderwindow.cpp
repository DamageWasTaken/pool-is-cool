#include <iostream>
#include <SDL3/SDL.h>

#include "RenderWindow.hpp"

float multiplier = 1;

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :window(NULL), renderer(NULL)
{
	w = p_w;
	h = p_h;
    SDL_CreateWindowAndRenderer(p_title, p_w, p_h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer);

    if ( NULL == window ) {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
    }
}

int RenderWindow::getRefreshRate() {
	SDL_DisplayID display_id = SDL_GetDisplayForWindow(window);

	const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(display_id);

    return mode->refresh_rate;
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::changeSize(int width, int height, float scale_ratio) 
{
	w = width;
	h = height;
    SDL_SetWindowSize(window, width, height);
    multiplier = scale_ratio;
}