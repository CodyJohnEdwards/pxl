#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <SDL2/SDL.h>
struct Velocity
{
    int right = 0;
    int left = 0;
    int up = 0;
    int down = 0;
};
struct Controls{
    bool left = false;
    bool right = false;
    bool jump = false;
};
class Controller
{
public:
    Velocity velocity;
    Controls controls;
    bool isJumping = false;
    bool active = false;
    Controller()
    {
        
    }
    void listen(){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                this->active = true;
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                case SDLK_w:
                    if (!isJumping)
                    {
                        isJumping = true;
                        velocity.up = 20;
                    }
                    break;
                case SDLK_a:
                    velocity.left = -10;
                    velocity.right = 0;
                    this->controls.left = true;
                    this->controls.right = false;
                    // flip = SDL_FLIP_HORIZONTAL;
                    break;
                case SDLK_d:
                    velocity.right = 10;
                    velocity.left = 0;
                    this->controls.right = true;
                    this->controls.left = false;
                    // flip = SDL_FLIP_NONE;
                    break;
                }
                break;
            case SDL_KEYUP:
                this->active = false;
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                case SDLK_w:
                    // velUp = 0;
                    break;
                case SDLK_a:
                    // playAnim = false;
                    velocity.right = 0;
                    velocity.left = 0;
                    break;
                case SDLK_d:
                    // playAnim = false;
                    velocity.right = 0;
                    velocity.left = 0;
                    break;
                }
                break;
                break;
            }
        }
        const Uint8 *keyboard_state_array;
        keyboard_state_array = SDL_GetKeyboardState(NULL);

        // Check if the 'A' key is being held down
        if (keyboard_state_array[SDL_SCANCODE_A])
        {
            // playAnim = true;
            velocity.left = -10;
            velocity.right = 0;
        }

        if (keyboard_state_array[SDL_SCANCODE_D])
        {
            // playAnim = true;
            velocity.left = 0;
            velocity.right = 10;
        }
    }
    void resetVelocity(){
        velocity.up = 0;
        velocity.down = 0;
        velocity.right = 0;
        velocity.left = 0;
    }

protected:
private:

};
#endif