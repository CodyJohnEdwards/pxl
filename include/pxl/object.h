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
        // SDL_RenderCopyEx(
        //     renderer_,
        //     this->texture_, 
        //     &this->sprites_[this->currentFrame_],
        //     &this->boundingBox_,
        //     0,
        //     NULL,
        //     this->flip_
        // );
        SDL_RenderCopy(renderer_, texture_, NULL, &boundingBox_);

    }

    SDL_Point getPosition(){
        return {boundingBox_.x,boundingBox_.y};
    }

    SDL_Rect getBoundingBox(){
        return boundingBox_;
    }

protected:
    int currentFrame_ = 0;
    SDL_RendererFlip flip_ = SDL_FLIP_NONE;
    SDL_Renderer *renderer_;
    SDL_Rect sprites_[4]; 
    SDL_Texture *texture_ = NULL;
    SDL_Rect boundingBox_ = {0,0,0,0};
};
#endif