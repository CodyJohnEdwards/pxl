#ifndef OBJECT_H
#define OBJECT_H
#include <SDL2/SDL.h>
class Object
{
public:
    Object(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h)
    {
        this->renderer_ = renderer;
        this->texture_ = texture;
        this->boundingBox_.x = x;
        this->boundingBox_.y = y;
        this->boundingBox_.h = h;
        this->boundingBox_.w = w;
        
    }
    
    void updateShader(int r, int g , int b){
        SDL_SetTextureColorMod(this->texture_,r,g,b);
    }

    void render(){
        SDL_RenderCopyEx(
            this->renderer_,
            this->texture_, 
            &this->sprites_[this->currentFrame_],
            &this->boundingBox_,
            0,
            NULL,
            this->flip_
        );
    }

    private:
    int currentFrame_ = 0;
    SDL_RendererFlip flip_ = SDL_FLIP_NONE;
    SDL_Renderer *renderer_;
    SDL_Rect sprites_[4]; 
    SDL_Texture *texture_ = NULL;
    SDL_Rect boundingBox_ = {0,0,0,0};
};
#endif