#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include <iostream>

class FontManager {
    public:
        FontManager(const char* p_filePath);
        void addFont(int p_size);
        void cleanUp();
        TTF_Font* get(int p_size)
        {
            auto it = fonts.find(p_size);

            if (it != fonts.end()) {
                return it->second;
            }

            std::cout << "Font size not found: " << p_size << " Creating font at size that size" << std::endl;

            fonts.insert({p_size, TTF_OpenFont(font_file_path, p_size)});

            return fonts.at(p_size);
        }
    private:
        const char* font_file_path;
        std::unordered_map<int, TTF_Font*> fonts;
};