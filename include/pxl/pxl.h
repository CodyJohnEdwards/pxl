#ifndef PXL_H
#define PXL_H
#include <pxl/object.h>
#include <string>

enum PxlType
{
    BLANK,
    DIRT,
    ROCK,
    GRASS
};

class Pxl : public Object
{
private:

public:
    PxlType type = BLANK;
    Pxl(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h) : Object(renderer, texture, x, y, w, h) {}
};
#endif