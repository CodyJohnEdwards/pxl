#ifndef ENTITY_H
#define ENTITY_H
#include <pxl/object.h>
class Entity : public Object
{
private:
    int currentFrame_ = 0;
    SDL_RendererFlip flip_ = SDL_FLIP_NONE;
    SDL_Rect sprites[4]; 
public:
    Entity(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h) : Object(renderer, texture, x, y, w, h) {
         for (int i = 0; i < 4; i++)
        {
            sprites[i].x = i * 32; // Each sprite is 64 pixels wide
            sprites[i].y = 0;      // All sprites are on the finorst row
            sprites[i].w = 32;     // Each sprite is 64 pixels wide
            sprites[i].h = 54;     // Each sprite is 64 pixels tall
        }
    }

    void render() override
    {
        SDL_RenderCopyEx(
            renderer_,
            texture_,
            &sprites[currentFrame_],
            &boundingBox_,
            0,
            NULL,
            flip_);
    }
};
#endif