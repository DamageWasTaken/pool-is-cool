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
        void print();
        void render(RenderWindow& window, TextureManager& p_texture_manager);
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

        float get_mass(){
            return mass;
        }

        void setPosition(Vector2f new_position)
        {
            position = new_position; 
        }

        void setVelocity(Vector2f new_velocity)
        {
            velocity = new_velocity;
        }
        
        float getRadius(){
            return radius;
        }
  
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
        float getDiameter()
        {
            return diameter;
        }
        void setDiameter(float new_diameter)
        {
            diameter = new_diameter;
        }
         

    private:
        SDL_Texture* texture;
        float radius;
        Vector2f position;
        int rotation_state;
        Vector2f velocity;
        int effect_state;
        float diameter = 25.0f;
        float mass = std::powf(radius, 2)*M_PI;
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
        void setBallVelocities(Vector2f velocity); //Changes all ball velocities to a given velocity
        void state_change(int p_state); //Changes all balls at once to next state or a given state
  
    private:
        std::unordered_map<int, Ball> balls;
        TextureManager& s_texture_manager;
};

//For anything classed as "helper" functions
class BallUtils {
    public:
        BallUtils(TextureManager& p_texture_manager, BallManager& p_ball_manager, Vector2f& p_window_size);
        void render(RenderWindow& window);
        void handleMouseInput(Vector2f mouse, bool mouse_down);
        void setSpin(Vector2f new_spin);
        void initializeBalls(BallManager& ball_manager, Vector2f p_position = Vector2f(790.0f, 360.0f), float spacing = 25.0f);
        bool ballsMoving();
        void updateTexture(const char* texture_name, SDL_Texture* new_texture)
        {
            if (textures.find(texture_name) != textures.end())
            {
                textures[texture_name] = new_texture;
            } else {
                std::cout << "Texture not found: " << texture_name << std::endl;
            }
        }
        void updateCue(Vector2f new_position, float new_rotation);
        void updateCue(Vector2f new_position);
        void updateCue(float new_rotation);
        Vector2f getCuePosition() {
            return s_cue_position;
        }
        float getCueRotation() {
            return s_cue_rotation;
        }
        bool isSpinLocked() {
            return spin_lock;
        }
        void toggleSpinLock(bool p_state);
        void ballsStopped();
        void setInitialMousePosition(Vector2f mouse);
        Vector2f getInitialMousePos() {
            return initial_mouse_position;
        }
        void setPower(float new_power);
        float getPower() {
            return power;
        }
    private:
        TextureManager& s_texture_manager;
        BallManager& s_ball_manager;
        std::unordered_map<std::string, SDL_Texture*> textures;
        Vector2f spinOffset;
        Vector2 utils_border_buffer = Vector2(30, 30);
        Vector2f& window_size;
        Vector2f initial_mouse_position;
        Vector2f s_cue_position; // This is expresed as position of the tip
        float s_cue_rotation; // In degrees, 0 is default orientation, so right
        float power = 0.0f; // 0-100
        bool spin_lock = false;
        bool balls_stopped = true;
};
