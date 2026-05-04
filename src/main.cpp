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
#define POWER_DIVDER 2
#define POWER_MULTIPLIER 20.0f

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

static float width = window.getWindowSize().x;
static float height = window.getWindowSize().y;

static TextureManager texture_manager(window);

static BallManager ball_manager(texture_manager);

static std::vector<Vector2f> area_corners = {
    Vector2f(33, 159),
    Vector2f(67, 129),
    Vector2f(105, 166),
    Vector2f(510, 164),
    Vector2f(523, 122),
    Vector2f(553, 122),
    Vector2f(572, 166),
    Vector2f(974, 163),
    Vector2f(1008, 130),
    Vector2f(1043, 167),
    Vector2f(1012, 197),
    Vector2f(1012, 523),
    Vector2f(1040, 551),
    Vector2f(1005, 586),
    Vector2f(979, 557),
    Vector2f(571, 557),
    Vector2f(556, 596),
    Vector2f(523, 595),
    Vector2f(509, 560),
    Vector2f(107, 557),
    Vector2f(64, 590),
    Vector2f(36, 553),
    Vector2f(69, 522),
    Vector2f(70, 202)
};

static Area tabel_area(area_corners);

static BallUtils ball_utils(texture_manager, ball_manager, window_size);

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
        
        window.renderArea(tabel_area);
        window.renderCenter(table_center.x, table_center.y, texture_manager.get("pool_table"));
        
        ball_manager.render(window);

        if(!ball_utils.ballsMoving()){
            ball_utils.render(window);
        }
        
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
    Vector2f cueball_position = ball_manager.getBall(0).getPosition();

    switch (game_state)
    {
    case MENU:
        // Update menu

        break;

    case PLAYING:
        // Update game
        
        //Find the cueball
        ball_utils.updateCue(cueball_position);

        physics_handler.updatePhysics(frame_time);

        if (!ball_utils.ballsMoving())
        {
            ball_utils.ballsStopped();
        }
        

        break;

    case PAUSED:
        // Update pause screen

        break;

    default:
        break;
    }
}

bool input(SDL_Event event, Vector2f mouse, bool mouse_down)
{
    bool mouse_clicked = false;
    float cue_rotation = SDL_atan2f(mouse.y - ball_utils.getCuePosition().y, mouse.x - ball_utils.getCuePosition().x) * 180 / M_PI - 180.0f;

    if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !mouse_down)
    {
        mouse_down = true;
        mouse_clicked = true;
    }

    if (event.button.button == SDL_BUTTON_LEFT && event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        mouse_down = false;
    }

    switch (game_state)
    {
    case MENU:
        // Check input on menu

        break;
        
    case PLAYING:
        // Check input while playing

        if (!ball_utils.isSpinLocked()) {
            ball_utils.updateCue(cue_rotation);
        }

        if (mouse_clicked)
        {
            ball_utils.setInitialMousePosition(mouse);
            ball_utils.handleMouseInput(mouse, mouse_down);
        }

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if (event.key.key == SDLK_R)
            {
                ball_utils.setSpin(Vector2f(0.0f, 0.0f));
                ball_utils.toggleSpinLock(false);
            }
        }

        if (mouse_down)
        {
            ball_utils.handleMouseInput(mouse, mouse_down);
        }

        if(!ball_utils.ballsMoving() && ball_utils.isSpinLocked() && ball_utils.getPower() > 5.0f && !mouse_down){
            float power = ball_utils.getPower() * POWER_MULTIPLIER;
            Vector2f shot_vel = scaleVector2f(rotateVector2f(Vector2f(1, 0), ball_utils.getCueRotation()), power); 
            ball_manager.getBall(0).setVelocity(shot_vel);
            ball_utils.toggleSpinLock(false);
        };

        break;

    case PAUSED:
        // Check input on pause screen

        break;

    default:
        break;
    }

    
    

    return mouse_down;
}

int main( int argc, char *argv[] ) 
{
    srand(time(NULL));

    //Force max window size
    //window.scaleToScreen();

    window_size = window.getWindowSize();

    ball_utils.initializeBalls(ball_manager, Vector2f(window_size.x*(79.0f/110.0f)+25.0f, window_size.y/2), 25.0f);
    ball_manager.addBall(window_size.x*(1-79.0f/110.0f), window_size.y/2, 0);

    /*
    for(int i = 0; i < 16; i++){
        ball_manager.getBall(i).setVelocity((Vector2f){500.f, 500.f});
    }*/

    ball_manager.state_change(0);

    int window_refresh_rate = window.getRefreshRate();

    std::cout << "Window Refresh Rate: " << window_refresh_rate << std::endl;

    const float delta_time = 0.01f;
    float accumulator = 0.0f;
    float effect_accumulator = 0.0f;
    float current_time = utils::hireTimeInSeconds();
        
    bool mouse_down = false;

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
            Vector2f mouse;
            SDL_GetMouseState(&mouse.x, &mouse.y);

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT) 
                {
                    game_running = false;
                }
                mouse_down = input(event, mouse, mouse_down);
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
