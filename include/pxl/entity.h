#ifndef ENTITY_H
#define ENTITY_H
#include <pxl/object.h>
class Entity : public Object{
    public:

    Entity(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h) : Object(renderer,texture,x,y,w,h){}
    
    void render() override { 
         SDL_RenderCopyEx(
            renderer_,
            texture_, 
            &sprites_[Object::currentFrame_],
            &boundingBox_,
            0,
            NULL,
            flip_
        );
    }
};
#endif