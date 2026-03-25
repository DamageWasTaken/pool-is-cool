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

    private:
        SDL_Texture* texture;
        Vector2f position;
        int rotation_state;
        Vector2f velocity;
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

    private:
        std::unordered_map<int, Ball> balls;
        TextureManager& s_texture_manager;
};
