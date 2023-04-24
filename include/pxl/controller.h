#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <pxl/renderer.h>
struct Velocity
{
    int right;
    int left;
    int up;
    int down;
};
class Controller
{
public:
    Velocity velocity;
    Controller(Renderer &renderer) : renderer_(renderer)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                case SDLK_w:
                    // if (!isJumping)
                    // {
                        // isJumping = true;
                        velocity.up = 20;
                    // }
                    break;
                case SDLK_a:
                    velocity.left = -10;
                    velocity.right = 0;
                    // flip = SDL_FLIP_HORIZONTAL;
                    break;
                case SDLK_d:
                    // cameraRect.x += velPos;
                    velocity.right = 10;
                    velocity.left = 0;
                    // flip = SDL_FLIP_NONE;
                    break;
                }
                break;
            case SDL_KEYUP:
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

protected:
private:
  Renderer& renderer_;
};
#endif