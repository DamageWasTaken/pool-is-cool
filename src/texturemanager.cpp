#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "TextureManager.hpp"
#include "RenderWindow.hpp"

#include <filesystem>

TextureManager::TextureManager(RenderWindow& window)
{
    bool all_textures_loaded = true;
    std::string path = "res/gfx";
    for (const auto & entry : std::filesystem::recursive_directory_iterator(path)) {
        std::string file_path = entry.path().string();
        if (file_path.find("exclude") != std::string::npos || entry.is_directory() || entry.path().extension() != ".png") {
            continue;
        }

        std::string file_name = entry.path().filename().string();
        //Cut off the .png
        file_name.resize(file_name.size()-4);

        SDL_Texture* texture = window.loadTexture(file_path.c_str());

        if (texture) {
            textures.insert({file_name, texture});
            std::cout << "Loaded texture: " << std::left << std::setw(40) << file_path << " | Unique name: " << file_name << std::endl;
        } else {
            std::cout << "Failed to load texture: " << file_path << std::endl;
            all_textures_loaded = false;
            continue;
        }
    }

    if (all_textures_loaded) {
        std::cout << "All textures have been loaded successfully" << std::endl;
    }
}

SDL_Texture* TextureManager::get(const std::string& p_name)
{
    auto it = textures.find(p_name);

    if (it != textures.end()) {
        return it->second;
    }

    std::cout << "Texture not found: " << p_name << std::endl;

    return nullptr;
}