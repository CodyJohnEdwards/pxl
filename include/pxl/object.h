#ifndef OBJECT_H
#define OBJECT_H
#include <SDL2/SDL.h>
class Object
{
public:
    Object(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h)
    {
        renderer_ = renderer;
        texture_ = texture;
        boundingBox_.x = x;
        boundingBox_.y = y;
        boundingBox_.h = h;
        boundingBox_.w = w;

    }
    
    void setShader(int r, int g , int b){
        SDL_SetTextureColorMod(this->texture_,r,g,b);
        render();
    }

    virtual void render(){
        SDL_RenderCopy(renderer_, texture_, NULL, &boundingBox_);
    }

    SDL_Point getPosition(){
        return {boundingBox_.x,boundingBox_.y};
    }

    SDL_Rect getBoundingBox(){
        return boundingBox_;
    }

    void setPosition(int x, int y){
        boundingBox_.x = x;
        boundingBox_.y = y;
    }

protected:
    SDL_Renderer *renderer_;
    SDL_Texture *texture_ = NULL;
    SDL_Rect boundingBox_ = {0,0,0,0};
};
#endif