#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Ball.hpp"
#include "TextureManager.hpp"
#include "RenderWindow.hpp"

#define BALL_AMOUNT 15

Ball::Ball(TextureManager& p_texture_manager, float p_x, float p_y, std::string p_texture_name)
    :position(p_x, p_y), rotation_state(0)
{
    std::string texture_name = p_texture_name + "_" + std::to_string(rotation_state);
    texture = p_texture_manager.get(texture_name);
}

void Ball::render(RenderWindow& window, TextureManager& p_texture_manager){
    SDL_Texture *shadow;
    SDL_Texture *shine;

    if (effect_state%2 == 0)
        shadow = p_texture_manager.get("shadow");
    else
        shadow = p_texture_manager.get("shadow_alt");

    if (std::floor(effect_state/2) == 0)
        shine = p_texture_manager.get("shine");
    else
        shine = p_texture_manager.get("shine_alt");

    window.render(*this, shadow, shine);
}

BallManager::BallManager(TextureManager& p_texture_manager)
    :s_texture_manager(p_texture_manager)
{
    /*s_texture_manager.get("shadow");
    s_texture_manager.get("shine");
    s_texture_manager.get("shadow_alt");
    s_texture_manager.get("shine_alt");*/
}

void BallManager::addBall(float p_x, float p_y, int ball_number) 
{
    if (ball_number > BALL_AMOUNT)
        return;

    std::string ball_name = "ball_" + std::to_string(ball_number);

    if (ball_number == 0)
        ball_name = "ball_cue";

    Ball temp_ball(s_texture_manager, p_x, p_y, ball_name);
    balls.insert({ball_number, temp_ball});
}

void BallManager::render(RenderWindow& p_window)
{
    for (auto& pair : balls) {
        Ball& ball = pair.second;
        ball.render(p_window, s_texture_manager);
    }
}
