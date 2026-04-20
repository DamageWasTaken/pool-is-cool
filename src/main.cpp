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

static Vector2f window_size = Vector2f(1080, 720);

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

static RenderWindow window("Pool", window_size.x, window_size.y);

static TextureManager texture_manager(window);

static BallManager ball_manager(texture_manager);

static BallUtils ball_utils(texture_manager, window_size);

static Area tabel_area(Vector2f(window_size.x/2, window_size.y/2), window_size.x, window_size.y);

static PhysicsHandler physics_handler(ball_manager, tabel_area);

static float frame_time = 0;

static Uint64 current_tick = SDL_GetPerformanceCounter();
static Uint64 last_tick = 0;
static double delta_time_test = 0;

static bool game_running = true;

static int game_state = PLAYING;

void graphics()
{
    window.clear();

    const Vector2f screen_size = window.getWindowSize();
    const Vector2f table_center = Vector2f(screen_size.x/2, screen_size.y/2);

    switch (game_state)
    {
    case MENU:
        // Render menu

        break;

    case PLAYING:
        // Render game
        
        ball_utils.render(window);

        /*
        window.renderCenter(screen_size.x-30, 30, texture_manager.get("ball_cue_-1"),2);
        window.renderCenter(screen_size.x-30, 30, texture_manager.get("ball_dot"),2);
        */

        window.renderCenter(table_center.x, table_center.y, texture_manager.get("pool_table"));

        ball_manager.render(window);
        
        break;

    case PAUSED:
        // Render pause screen

        break;
    
    default:
        break;
    }

    window.display();
}

void update()
{
    switch (game_state)
    {
    case MENU:
        // Update menu

        break;

    case PLAYING:
        // Update game
        physics_handler.updatePhysics(frame_time); 
        break;

    case PAUSED:
        // Update pause screen

        break;

    default:
        break;
    }
}

bool input(SDL_Event event, Vector2f mouse, bool mouse_clicked)
{
    bool mouse_down = false;
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        mouse_down = true;
        mouse_clicked = true;
    }

    switch (game_state)
    {
    case MENU:
        // Check input on menu

        break;
        
    case PLAYING:
        // Check input while playing

        if (mouse_down)
        {
            ball_utils.handleMouseInput(mouse);
        }

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if (event.key.key == SDLK_R)
            {
                ball_utils.setSpin(Vector2f(0.0f, 0.0f));
            }
        }

        break;

    case PAUSED:
        // Check input on pause screen

        break;

    default:
        break;
    }

    return mouse_clicked;
}

int main( int argc, char *argv[] ) 
{
    srand(time(NULL));

    //window.scaleToScreen();

    window_size = window.getWindowSize();

    for (int i = 0; i < 16; i++)
    {
        ball_manager.addBall(100+i%5*100, 100+floor(i/5)*100, i);
    }

    ball_manager.state_change(0);

    int window_refresh_rate = window.getRefreshRate();

    tabel_area = Area(Vector2f(0.0f, 0.0f), window_size);

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
            bool mouse_clicked = false;

            Vector2f mouse;
            SDL_GetMouseState(&mouse.x, &mouse.y);

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT) 
                {
                    game_running = false;
                }
                mouse_clicked = input(event, mouse, mouse_clicked);
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
