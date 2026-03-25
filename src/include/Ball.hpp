#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

#include "Math.hpp"

class TextureManager;
class RenderWindow;

class Ball {
    public:
        Ball(TextureManager& p_texture_manager, float p_x, float p_y, std::string p_texture_name);
        SDL_Texture* getTexture()
        {
            return texture;
        }
        Vector2f getPosition()
        {
            return position;
        }
        Vector2f getVelocity()
        {
            return velocity;
        }

        void setPosition(Vector2f new_position)
        {
            position = new_position; 
        }
        void setVelocity( Vector2f new_velocity)
        {
            velocity = new_velocity;
        }

        void render(RenderWindow& window, TextureManager& p_texture_manager);
  
        void change_state(int p_state = -1)
        {
            //-1 state means to just go to the next state
            if (p_state == -1)
                p_state = (effect_state + 1) % 4;
            
            //Clamp the state between 0 and 4 just in case
            p_state = std::min(p_state, 4);
            p_state = std::max(p_state, 0);

            effect_state = p_state;
        }

    private:
        SDL_Texture* texture;
        Vector2f position;
        int rotation_state;
        Vector2f velocity;
        int effect_state;
};

class BallManager {
    public:
        BallManager(TextureManager& p_texture_manager);
        void addBall(float p_x, float p_y, int ball_number);
        void render(RenderWindow& p_window);
        Ball& getBall(int p_ball_number)
        {
             return balls.at(p_ball_number);
        };
        int getBallAmount()
        {
            return balls.size();
        };
        void state_change(int p_state); //Changes all balls at once to next state or a given state
  
    private:
        std::unordered_map<int, Ball> balls;
        TextureManager& s_texture_manager;
};
