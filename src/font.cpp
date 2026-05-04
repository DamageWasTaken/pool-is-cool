#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <unordered_set>

#include "Font.hpp"

FontManager::FontManager(const char* p_filePath)
    :font_file_path(p_filePath)
{

}

void FontManager::addFont(int p_size)
{
    if (fonts.find(p_size) == fonts.end()) {
        fonts.insert({p_size, TTF_OpenFont(font_file_path, p_size)});
    }
}

void FontManager::cleanUp()
{
    for (auto& pair : fonts) {
        TTF_CloseFont(pair.second);
    }
}