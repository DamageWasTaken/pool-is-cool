#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "TextureManager.hpp"
#include "Physics.hpp"
#include "Ball.hpp"
#include "Area.hpp"
#include "Utils.hpp"

#define EFFECT_CHANGE_RATE 1.0f

static int WIDTH = 1080, HEIGHT = 720;

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

static bool SDLinit = init();

static RenderWindow window("Pool", WIDTH, HEIGHT);

static TextureManager texture_manager(window);

static BallManager ball_manager(texture_manager);

static Area tabel_area(Vector2f(WIDTH/2, HEIGHT/2), WIDTH, HEIGHT);

static PhysicsHandler physics_handler(ball_manager, tabel_area);

static float frame_time = 0;

static Uint64 current_tick = SDL_GetPerformanceCounter();
static Uint64 last_tick = 0;
static double delta_time_test = 0;

static bool game_running = true;

void graphics()
{
    window.clear();

    ball_manager.render(window);

    window.display();
}

void update()
{
    physics_handler.updatePhysics(frame_time); 
}

void input(SDL_Event event)
{
    //Input calls here
}

int main( int argc, char *argv[] ) 
{
    srand(time(NULL));
    for (int i = 0; i < 16; i++)
    {
        ball_manager.addBall(600+i%5*500, 600+floor(i/5)*500, i);
    }

    ball_manager.state_change(0);

    int window_refresh_rate = window.getRefreshRate();

    tabel_area = Area(Vector2f(0.0f, 0.0f), Vector2f(WIDTH, HEIGHT));

    std::cout << "Window Refresh Rate: " << window_refresh_rate << std::endl;

    const float delta_time = 0.01f;
    float accumulator = 0.0f;
    float effect_accumulator = 0.0f;
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
        //std::cout << new_time << " - " << frame_time << " - " << current_time << " - " << accumulator << " - " << delta_time_test << std::endl;

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

        //Effects
        effect_accumulator += frame_time;
        if (effect_accumulator >= EFFECT_CHANGE_RATE) {
            effect_accumulator = 0.0f;
            ball_manager.state_change(-1);
        }
        const float alpha = accumulator / delta_time;

        int frame_ticks = SDL_GetTicks() - start_ticks;

        if (frame_ticks < 1000 / window.getRefreshRate()) {
            SDL_Delay(1000 / window.getRefreshRate() - frame_ticks);
        }

    }

    window.cleanUp();
    SDL_Quit();
    TTF_Quit();

    return EXIT_SUCCESS;
}
