#ifndef CAMERA_H
#define CAMERA_H
#include <pxl/controller.h>
class Camera : public Controller
{
    SDL_Rect renderBox = {
        0, 0, 1280, 720};
    Camera(Renderer &renderer) : Controller(renderer)
    {
    }
    
};
#endif