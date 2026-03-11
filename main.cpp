#include <iostream>
#include <SDL3/SDL.h>

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Utils.hpp"

int WIDTH = 1080, HEIGHT = 720;

bool init()
{
    //Used only for Linux Ubuntu, if not used, no sound will be played
    //SDL_AudioInit("ALSA");
	if (!SDL_Init(SDL_INIT_VIDEO))
		std::cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
	/*Needed later
    if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        std::cout << "SDL mixer has failed. Error: " << Mix_GetError() << std::endl;
    if (!(TTF_Init() > -1))
		std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;*/
	std::cout << "Everything has initialized sucessfully!" << std::endl;
    return true;
}

bool SDLinit = init();

RenderWindow window("Pool", WIDTH, HEIGHT);

void graphics()
{

}

float frameTime = 0;

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTimeTest = 0;

bool gameRunning = true;

int main( int argc, char *argv[] ) 
{

    int windowRefreshRate = window.getRefreshRate();

    std::cout << "Window Refresh Rate: " << windowRefreshRate << std::endl;

    const float deltaTime = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    while(gameRunning)
    {
        lastTick = currentTick;
        currentTick = SDL_GetPerformanceCounter();
        deltaTimeTest = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );

        int startTicks = SDL_GetTicks();

        float newTime = utils::hireTimeInSeconds();
        frameTime = newTime - currentTime;

        currentTime = newTime;

        accumulator += frameTime;

        //DEBUG - Enable for frame debug
        //std::cout << newTime << " - " << frameTime << " - " << currentTime << " - " << accumulator << " - " << std::endl;

        SDL_Event event;

        while (accumulator >= deltaTime)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT) 
                {
                    gameRunning = false;
                }
            }
            accumulator -= deltaTime;
        }

        graphics();
        
        const float alpha = accumulator / deltaTime;

        int frameTicks = SDL_GetTicks() - startTicks;

        if (frameTicks < 1000 /  window.getRefreshRate()) {
            SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
        }

    }

    window.cleanUp();
    SDL_Quit();

    return EXIT_SUCCESS;
}