#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Math.hpp"
#include "Area.hpp"
#include "Ball.hpp"

class RenderWindow {
    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        int getRefreshRate();
        SDL_Texture* loadTexture(const char* p_filePath);
        void render(float x, float y, SDL_Texture* p_tex);
        void render(Ball& ball, SDL_Texture* p_shadow_texture, SDL_Texture* p_shine_texture);
        void renderArea(Area& area);
        void renderCenter(float x, float y, SDL_Texture* p_tex);
        void renderCenter(float x, float y, SDL_Texture* p_tex, float scale);
        void renderRotated(float x, float y, SDL_Texture* p_tex, float angle, Vector2f rotation_point);
        void renderMasked(float x, float y, SDL_Texture* p_tex, SDL_Texture* p_mask, float scale = 1.0f, Vector2f mask_offset = Vector2f(0.0f, 0.0f));
        void cleanUp();
        void clear();
        void display();
        void scaleToScreen();
        void changeSize(int width, int height, float scale_ratio);
        Vector2f getWindowSize()
        {
            return Vector2f(w, h);
        }
    private:
        int w, h;
        SDL_Window* window;
        SDL_Renderer* renderer;
};

