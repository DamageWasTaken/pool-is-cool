#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "TextureManager.hpp"
#include "Ball.hpp"
#include "Utils.hpp"

int WIDTH = 1080, HEIGHT = 720;

bool init()
{
    //Used only for Linux Ubuntu, if not used, no sound will be played
    //SDL_AudioInit("ALSA");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "SDL_Init HAS FAILED: " << SDL_GetError() << std::endl;
        return false;
    }
    if (!MIX_Init()) {
        std::cout << "MIX_Init HAS FAILED: " << SDL_GetError() << std::endl;
        return false;
    }
    if (!TTF_Init()) {
        std::cout << "TTF_init HAS FAILED: " << SDL_GetError() << std::endl;
        return false;
    }
	std::cout << "Everything has initialized sucessfully!" << std::endl;
    return true;
}

bool SDLinit = init();

RenderWindow window("Pool", WIDTH, HEIGHT);

TextureManager texture_manager(window);

BallManager ball_manager(texture_manager);

void graphics()
{
    window.clear();

    ball_manager.render(window);
    //Draw calls here

    window.display();
}

void update()
{
    //Update calls here
}

void input(SDL_Event event)
{
    //Input calls here
}

float frame_time = 0;

Uint64 current_tick = SDL_GetPerformanceCounter();
Uint64 last_tick = 0;
double delta_time_test = 0;

bool game_running = true;


int main( int argc, char *argv[] ) 
{
    ball_manager.addBall(300, 300, 1);
    ball_manager.addBall(600, 600, 0);

    int window_refresh_rate = window.getRefreshRate();

    std::cout << "Window Refresh Rate: " << window_refresh_rate << std::endl;

    const float delta_time = 0.01f;
    float accumulator = 0.0f;
    float current_time = utils::hireTimeInSeconds();

    while(game_running)
    {
        last_tick = current_tick;
        current_tick = SDL_GetPerformanceCounter();
        delta_time_test = (double)((current_tick - last_tick)*1000 / (double)SDL_GetPerformanceFrequency() );

        int start_ticks = SDL_GetTicks();

        float new_time = utils::hireTimeInSeconds();
        frame_time = new_time - current_time;

        current_time = new_time;

        accumulator += frame_time;

        //DEBUG - Enable for frame debug
        //std::cout << new_time << " - " << frame_time << " - " << current_time << " - " << accumulator << std::endl;

        SDL_Event event;

        while (accumulator >= delta_time)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT) 
                {
                    game_running = false;
                }
                input(event);
            }
            accumulator -= delta_time;
        }

        update();
        graphics();
        
        const float alpha = accumulator / delta_time;

        int frame_ticks = SDL_GetTicks() - start_ticks;

        if (frame_ticks < 1000 /  window.getRefreshRate()) {
            SDL_Delay(1000 / window.getRefreshRate() - frame_ticks);
        }

    }

    window.cleanUp();
    SDL_Quit();
    TTF_Quit();

    return EXIT_SUCCESS;
}