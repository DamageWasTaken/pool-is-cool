#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>

#include "Ball.hpp"
#include "TextureManager.hpp"
#include "RenderWindow.hpp"

#define BALL_AMOUNT 15

Ball::Ball(TextureManager& p_texture_manager, float p_x, float p_y, std::string p_texture_name)
    :position(p_x, p_y), rotation_state(0)
{
    std::string texture_name = p_texture_name + "_" + std::to_string(rotation_state);
    texture = p_texture_manager.get(texture_name);
    effect_state = rand() % 4;
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

}

void BallManager::addBall(float p_x, float p_y, int p_ball_number) 
{
    if (p_ball_number > BALL_AMOUNT)
        return;

    std::string ball_name = "ball_" + std::to_string(p_ball_number);

    if (p_ball_number == 0)
        ball_name = "ball_cue";

    Ball temp_ball(s_texture_manager, p_x, p_y, ball_name);
    balls.insert({p_ball_number, temp_ball});
}

void BallManager::render(RenderWindow& p_window)
{
    for (auto& pair : balls) {
        Ball& ball = pair.second;
        ball.render(p_window, s_texture_manager);
    }
}
      
void BallManager::state_change(int p_state = -1)
{
    for (auto& pair : balls) {
        Ball& ball = pair.second;
        ball.change_state(p_state);
    }
}


BallUtils::BallUtils(TextureManager& p_texture_manager, Vector2f& p_window_size)
:spinOffset(0.0f, 0.0f), s_texture_manager(p_texture_manager), window_size(p_window_size)
{
    spin_marker_texture = s_texture_manager.get("ball_dot");
    ball_texture = s_texture_manager.get("ball_cue_-1");
}

void BallUtils::render(RenderWindow& p_window)
{
    Vector2f window_size = p_window.getWindowSize();
    Vector2f position = Vector2f(window_size.x - utils_border_buffer.x, utils_border_buffer.y);
    p_window.renderCenter(position.x, position.y, ball_texture, 2);
    p_window.renderCenter(position.x+spinOffset.x, position.y+spinOffset.y, spin_marker_texture, 2);
}

void BallUtils::handleMouseInput(Vector2f mouse)
{
    float max_offset = 21.0f;
    float distance_to_center = lengthVector2f(subtractVector2f(mouse, Vector2f(window_size.x - utils_border_buffer.x, utils_border_buffer.y)));
    if (distance_to_center <= max_offset)
    {
        spinOffset = subtractVector2f(mouse, Vector2f(window_size.x - utils_border_buffer.x, utils_border_buffer.y));
    }
}

void BallUtils::setSpin(Vector2f new_spin)
{
    spinOffset = new_spin;
}