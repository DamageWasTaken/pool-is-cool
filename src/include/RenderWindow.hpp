#pragma once
#include <SDL3/SDL.h>
#include "Math.hpp"

class RenderWindow {
    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        int getRefreshRate();
        void cleanUp();
        void clear();
        void display();
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

