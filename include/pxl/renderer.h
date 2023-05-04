#ifndef RENDERER_H
#define RENDERER_H
#include <pxl/core.h>
#include <pxl/camera.h>
#include <pxl/controller.h>
class Renderer{
public:
    Core core;
    Renderer(){
        double **terrainMap = core.generateTerrainMap();

        //light rect
        SDL_Rect rect = {
            1280 / 2,
            720 / 2,
            5,
            5};

        
        //LIGHT RECT TEXTURE Load the texture image
        SDL_Texture *texture = NULL;
        std::string textureFile = "text.png";
        SDL_Surface *surface = SDL_LoadBMP("./texture.bmp");
        SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface);

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        // PLAYER SPRITE SHEET
        //  Load the sprite sheet into a texture
        SDL_Surface *spriteSheetSurface = IMG_Load("32bit.png");
        SDL_Texture *spriteSheetTexture = SDL_CreateTextureFromSurface(renderer, spriteSheetSurface);
        SDL_FreeSurface(spriteSheetSurface);

        // Define the position and size of each individual sprite
        SDL_Rect sprites[4];
        for (int i = 0; i < 4; i++)
        {
            sprites[i].x = i * 32; // Each sprite is 64 pixels wide
            sprites[i].y = 0;      // All sprites are on the finorst row
            sprites[i].w = 32;     // Each sprite is 64 pixels wide
            sprites[i].h = 54;     // Each sprite is 64 pixels tall
        }

        // Define the current frame of the animation
        int currentFrame = 0;
        int frameTime = 100; // Time between frames in milliseconds
        Uint32 lastFrameTime = SDL_GetTicks();


        // Draw the current frame of the animation onto the screen
        SDL_Rect characterDestRect = {0, 0, 32, 54};

        
    

        Camera camera = Camera();
        Controller controller = Controller();

        
        
        
        
        
        // Run the game loop
        bool running = true;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        float jumpDuration = 0.5;

        //RENDERER
        while (running) 
        {
            controller.velocity.down = 10;

            SDL_Point playerPos = {
                rect.x,
                rect.y};

            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            SDL_RenderClear(renderer);
            controller.listen();
            if(controller.controls.left){
                flip = SDL_FLIP_HORIZONTAL;
            }
            if(controller.controls.right){
                flip = SDL_FLIP_NONE;
            }
        
            for (int row = 100; row < core.MAX_HEIGHT; row++)
            {
                for (int tile = 0; tile < core.MAX_WIDTH; tile++)
                {

                    int tileX = tile * 5;
                    int tileY = row * 5;
                    // check if tile is within renderRect
                    if (tileX >= camera.renderBox.x && tileX < camera.renderBox.x + camera.renderBox.w && tileY >= camera.renderBox.y && tileY < camera.renderBox.y + camera.renderBox.h)
                    {
                        // printf("rendering terrain");
                        if (terrainMap[row][tile] > 0)
                        {

                            SDL_Rect dstRect = {
                                tileX - camera.renderBox.x,
                                tileY - camera.renderBox.y,
                                5,
                                5};

                            SDL_Point rect2Pos = {
                                dstRect.x,
                                dstRect.y};

                            float d = core.distance(playerPos, rect2Pos);
                            float brightness = 1.0 - d / 100;
                            brightness = fmaxf(brightness, 0.0); // clamp @ 1 || 0

                            SDL_SetTextureColorMod(texture, brightness * 255, brightness * 255, brightness * 255);
                            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                            if (SDL_HasIntersection(&dstRect, &characterDestRect))
                            {
                                if (dstRect.y > characterDestRect.y - 27)
                                {
                                    controller.velocity.down = 0;
                                    SDL_SetTextureColorMod(texture, 0, 255, 0);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }
                                if (dstRect.x + 5 > characterDestRect.x + 16 && dstRect.y + 5 < characterDestRect.y + 54)
                                {
                                    controller.velocity.right = 0;
                                    controller.velocity.up = 10;
                                    controller.isJumping = true;
                                    SDL_SetTextureColorMod(texture, 255, 0, 0);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }

                                if (dstRect.x + 5 < characterDestRect.x + 16 && dstRect.y + 5 < characterDestRect.y + 54)
                                {
                                    controller.velocity.left = 0;
                                    controller.velocity.up = 10;
                                    controller.isJumping = true;
                                    SDL_SetTextureColorMod(texture, 0, 0, 255);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }

                                if (dstRect.y + 5 < characterDestRect.y + 27)
                                {
                                    controller.velocity.up = 0;
                                    SDL_SetTextureColorMod(texture, 255, 255, 0);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }
                            }
                        }
                    }
                }
            }

            if (controller.isJumping)
            {
                float dt = 1.0 / 60;
                // cameraRect.y -= velUp * dt;
                jumpDuration -= dt;
                controller.velocity.up -= dt;
                camera.renderBox.y += (controller.velocity.down - controller.velocity.up);
                // velUp -= 0.05;
                if (controller.velocity.up <= 0)
                {
                    controller.velocity.up = 0;
                }

                if (jumpDuration <= 0)
                {
                    controller .isJumping = false;
                    jumpDuration = 0.5;
                }
            }
            else
            {
                camera.renderBox.y += controller.velocity.down;
            }

            camera.renderBox.x += controller.velocity.right + controller.velocity.left;

            rect.x = camera.renderBox.w / 2 - 5 / 2;
            rect.y = camera.renderBox.h / 2 - 5 / 2;
            characterDestRect.x = camera.renderBox.w / 2 - 5 / 2;
            characterDestRect.y = camera.renderBox.h / 2 - 5 / 2;

            SDL_SetTextureColorMod(texture, 255, 255, 255);
            SDL_RenderCopyEx(renderer, spriteSheetTexture, &sprites[currentFrame], &characterDestRect, 0, NULL, flip);

            // Update the current frame of the animation based on elapsed time
            if (controller.active)
            {

                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastFrameTime >= frameTime)
                {
                    lastFrameTime = currentTime;
                    currentFrame = (currentFrame + 1) % 4; // There are 4 frames in this animation
                }
            }
            else
            {
                currentFrame = 0;
            }

            SDL_RenderCopy(renderer, texture, NULL, &rect);

            // Show the renderer
            SDL_RenderPresent(renderer);
        }

        // Clean up
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        // Deallocate the memory
        for (int y = 0; y < core.MAX_HEIGHT; y++)
        {
            // delete[] gradients[y];
            delete[] terrainMap[y];
        }
    }
private:
        SDL_Window *window = core.window;
        SDL_Renderer *renderer = core.renderer;
 
protected:
};
#endif // RENDERER_H