#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <unordered_set>

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


void BallUtils::initializeBalls(BallManager& ball_manager, Vector2f p_position, float spacing)
{
    std::unordered_set<int> ball_numbers;

    while (ball_numbers.size() < BALL_AMOUNT)
    {
        int random_number = rand() % BALL_AMOUNT + 1;
        ball_numbers.insert(random_number);
    }

    int middle_ball = std::ceil(BALL_AMOUNT / 2.0f);
    ball_numbers.erase(middle_ball);
    
    //Find first even and odd ball numbers to put in the corners
    int corner_1_ball = 0;
    int corner_2_ball = 0;

    for (auto it = ball_numbers.begin(); it != ball_numbers.end();)
    {
        if (corner_1_ball != 0 && corner_2_ball != 0)
        {
            break;
        }
        int value = *it;
        if (value < middle_ball && corner_1_ball == 0)
        {
            corner_1_ball = value;
            it = ball_numbers.erase(it);
        }
        else if (value > middle_ball && corner_2_ball == 0)
        {
            corner_2_ball = value;
            it = ball_numbers.erase(it);
        }
        else
        {
            ++it;
        }
    }

    std::cout << corner_1_ball << " - " << corner_2_ball << " - " << middle_ball << std::endl;

    int row_amount = (-1+sqrt(1+8*BALL_AMOUNT))/2;

    float s_x = p_position.x, s_y = p_position.y;

    int middle_ball_position = std::ceil(row_amount/2.0f)-1;

    for (int i = 0; i < row_amount; i++)
    {
        float total_row_height = i*spacing;
        s_y = p_position.y - total_row_height/2;    
        for (int j = 0; j <= i; j++)
        {
            int ball_number = 0;

            //Determine special balls
            if (i == row_amount-1 && j == 0)
            {
                ball_number = corner_1_ball;
            } else if (i == row_amount-1 && j == row_amount-1)
            {
                ball_number = corner_2_ball;
            } else if (i == middle_ball_position && j == middle_ball_position-1) {
                ball_number = middle_ball;
            } else {
                auto it = ball_numbers.begin();
                ball_number = *it;
                it = ball_numbers.erase(it);
            }
            
            ball_manager.addBall(s_x, s_y, ball_number);
            
            s_y += spacing;
        }
        s_x += spacing;
    }

}