#ifndef CAMERA_H
#define CAMERA_H
#include <SDL2/SDL.h>
class Camera
{
public:
    SDL_Rect renderBox = {
        0, 0, 1280, 720};
    Camera()
    {
    }
    
};
#endif