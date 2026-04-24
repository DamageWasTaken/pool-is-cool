#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <string>
#include "Math.hpp"

class RenderWindow;

class TextureManager {
    public:
        TextureManager(RenderWindow& window); // inits all textures
        SDL_Texture* get(const std::string& p_name);
        Vector2f getSize(const std::string& p_name);

    private:
        std::unordered_map<std::string, SDL_Texture*> textures;
};