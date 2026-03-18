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
    private:
        SDL_Texture* texture;
        Vector2f position;
        int rotation_state;
};

class BallManager {
    public:
        BallManager(TextureManager& p_texture_manager);
        void addBall(float p_x, float p_y, int ball_number);
        void render(RenderWindow& window);

    private:
        std::unordered_map<int, Ball> balls;
        TextureManager& s_texture_manager;
};