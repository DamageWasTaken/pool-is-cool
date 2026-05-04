#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "RenderWindow.hpp"

float multiplier = 1;

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
    :window(NULL), renderer(NULL)
{
	w = p_w;
	h = p_h;
    SDL_CreateWindowAndRenderer(p_title, p_w, p_h, /*SDL_WINDOW_RESIZABLE |*/ SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer);

    if ( NULL == window ) {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
    }
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) 
{
    SDL_Texture* texture = NULL;

    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL) {
        std::cout << "Failed to load texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

//Standard render
void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex)
{
	SDL_FRect src; 
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;

    SDL_GetTextureSize(p_tex, &src.w, &src.h);

	SDL_FRect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderTexture(renderer, p_tex, &src, &dst);
}

void RenderWindow::renderArea(Area& area) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (Edge& edge : area.edges) {
        SDL_RenderLine(
            renderer,
            (edge.start.x),
            (edge.start.y),
            (edge.end.x),
            (edge.end.y)
        );
    }
}

//Balls are always rendered from the center
void RenderWindow::render(Ball& ball, SDL_Texture* p_shadow_texture, SDL_Texture* p_shine_texture)
{
    SDL_FRect src; 
    src.x = 0;
    src.y = 0;
    src.w;
    src.h;

    SDL_GetTextureSize(ball.getTexture(), &src.w, &src.h);

    SDL_FRect dst;
    dst.x = (ball.getPosition().x - src.w/2);
    dst.y = (ball.getPosition().y - src.h/2);
    dst.w = src.w;
    dst.h = src.h;

	SDL_RenderTexture(renderer, ball.getTexture(), &src, &dst);
	SDL_RenderTexture(renderer, p_shadow_texture, &src, &dst);
	SDL_RenderTexture(renderer, p_shine_texture, &src, &dst);
}

//Renders from the center of the texture, not the top left corner
void RenderWindow::renderCenter(float p_x, float p_y, SDL_Texture* p_tex)
{
	SDL_FRect src;
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;

	SDL_GetTextureSize(p_tex, &src.w, &src.h);

	SDL_FRect dst;
	dst.x = (p_x - src.w/2);
	dst.y = (p_y - src.h/2);
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderTexture(renderer, p_tex, &src, &dst);
}

//Renders from the center of the texture, not the top left corner
void RenderWindow::renderCenter(float p_x, float p_y, SDL_Texture* p_tex, float scale)
{
	SDL_FRect src;
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;

	SDL_GetTextureSize(p_tex, &src.w, &src.h);

	SDL_FRect dst;
	dst.x = (p_x - src.w*scale/2);
	dst.y = (p_y - src.h*scale/2);
	dst.w = src.w * scale;
	dst.h = src.h * scale;

	SDL_RenderTexture(renderer, p_tex, &src, &dst);
}

int RenderWindow::getRefreshRate() {
	SDL_DisplayID display_id = SDL_GetDisplayForWindow(window);

	const SDL_DisplayMode *mode = SDL_GetDesktopDisplayMode(display_id);

    return mode->refresh_rate;
}

void RenderWindow::renderRotated(float x, float y, SDL_Texture* p_tex, float angle, Vector2f rotation_point)
{
	SDL_FRect src;
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;

	SDL_GetTextureSize(p_tex, &src.w, &src.h);

	SDL_FRect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_FPoint point;
	point.x = rotation_point.x;
	point.y = rotation_point.y;

	SDL_RenderTextureRotated(renderer, p_tex, &src, &dst, angle, &point, SDL_FLIP_NONE);
}

//Only renders a portion of the texture
void RenderWindow::renderClipped(float p_x, float p_y, SDL_Texture* p_tex, SDL_FRect clip_rect, float scale)
{
	Vector2f texture_size;
	SDL_GetTextureSize(p_tex, &texture_size.x, &texture_size.y);

	SDL_FRect src;
	src.x = clip_rect.x;
	src.y = clip_rect.y;
	src.w = clip_rect.w;
	src.h = clip_rect.h;

	

	SDL_FRect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = clip_rect.w * scale;
	dst.h = clip_rect.h * scale;

	SDL_RenderTexture(renderer, p_tex, &src, &dst);
}

void RenderWindow::scaleToScreen()
{
	SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(primaryDisplay);

	if (mode) {
		std::cout << "Primary display resolution: " << mode->w << " x " << mode->h << std::endl;
		w = mode->w;
		h = mode->h;

		SDL_SetWindowSize(window, w, h);
		SDL_SetWindowBordered(window, false);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}

void RenderWindow::renderText(float x, float y, const char* p_text, TTF_Font* p_font, SDL_Color p_color) 
{
	SDL_Surface* surface = TTF_RenderText_Blended(p_font, p_text, 0, p_color);
	if (!surface) {
		std::cout << "Failed to create text surface: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);

	if (!texture) {
		std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
		return;
	}

	render(x, y, texture);
	SDL_DestroyTexture(texture);
}

void RenderWindow::renderTextCenter(float x, float y, const char* p_text, TTF_Font* p_font, SDL_Color p_color) 
{
	SDL_Surface* surface = TTF_RenderText_Blended(p_font, p_text, 0, p_color);
	if (!surface) {
		std::cout << "Failed to create text surface: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroySurface(surface);

	if (!texture) {
		std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
		return;
	}

	renderCenter(x, y, texture);
	SDL_DestroyTexture(texture);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void RenderWindow::clear()
{
    //SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
    SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

//Leftover - Currently unused - Unsure if it will be used in the future
void RenderWindow::changeSize(int width, int height, float scale_ratio) 
{
	w = width;
	h = height;
    SDL_SetWindowSize(window, width, height);
    multiplier = scale_ratio;
}
