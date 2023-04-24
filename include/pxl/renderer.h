#ifndef RENDERER_H
#define RENDERER_H
#include <pxl/core.h>
class Renderer{
public:
    Core core;
    Renderer(){
        double **terrainMap = core.generateTerrainMap();

        SDL_Window *window = core.window;
        SDL_Renderer *renderer = core.renderer;
        SDL_Renderer *renderer2 = NULL;

        SDL_Rect rect = {
            1280 / 2,
            720 / 2,
            5,
            5};
        SDL_Rect rect2 = {
            50,
            50,
            5,
            5};

        // Load the texture image
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
            sprites[i].y = 0;      // All sprites are on the first row
            sprites[i].w = 32;     // Each sprite is 64 pixels wide
            sprites[i].h = 54;     // Each sprite is 64 pixels tall
        }

        // Define the current frame of the animation
        int currentFrame = 0;
        int frameTime = 100; // Time between frames in milliseconds
        Uint32 lastFrameTime = SDL_GetTicks();

        // Draw the current frame of the animation onto the screen
        SDL_Rect characterDestRect = {0, 0, 32, 54};

        SDL_Rect cameraRect = {
            0,
            0,
            1280,
            720};

        int velPos = 0;
        int velNeg = 0;
        int velUp = 0;
        int grav = 0;
        // Run the game loop
        bool running = true;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        bool playAnim = false;
        bool isJumping = false;
        float jumpDuration = 0.5;
        while (running)
        {
            grav = 10;
            // velPos = 10;
            // velNeg = 10;

            SDL_Point playerPos = {
                rect.x,
                rect.y};

            // Convert camera position to render coordinates
            SDL_Rect renderRect = {
                cameraRect.x,
                cameraRect.y,
                cameraRect.w,
                cameraRect.h};

            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            SDL_RenderClear(renderer);

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_SPACE:
                    case SDLK_w:
                        if (!isJumping)
                        {
                            isJumping = true;
                            velUp = 20;
                        }
                        break;
                    case SDLK_a:
                        // cameraRect.x -= velNeg;
                        velNeg = -10;
                        velPos = 0;
                        flip = SDL_FLIP_HORIZONTAL;
                        break;
                    case SDLK_d:
                        // cameraRect.x += velPos;
                        velPos = 10;
                        velNeg = 0;
                        flip = SDL_FLIP_NONE;
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
                        playAnim = false;
                        velPos = 0;
                        velNeg = 0;
                        break;
                    case SDLK_d:
                        playAnim = false;
                        velNeg = 0;
                        velPos = 0;
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
                playAnim = true;
                velNeg = -10;
                velPos = 0;
            }

            if (keyboard_state_array[SDL_SCANCODE_D])
            {
                playAnim = true;
                velNeg = 0;
                velPos = 10;
            }

            for (int row = 100; row < core.MAX_HEIGHT; row++)
            {
                for (int tile = 0; tile < core.MAX_WIDTH; tile++)
                {

                    int tileX = tile * 5;
                    int tileY = row * 5;

                    // check if tile is within renderRect
                    if (tileX >= renderRect.x && tileX < renderRect.x + renderRect.w && tileY >= renderRect.y && tileY < renderRect.y + renderRect.h)
                    {

                        if (terrainMap[row][tile] > 0)
                        {

                            SDL_Rect dstRect = {
                                tileX - renderRect.x,
                                tileY - renderRect.y,
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
                                    grav = 0;
                                    SDL_SetTextureColorMod(texture, 0, 255, 0);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }
                                if (dstRect.x + 5 > characterDestRect.x + 16 && dstRect.y + 5 < characterDestRect.y + 54)
                                {
                                    velPos = 0;
                                    velUp = 10;
                                    isJumping = true;
                                    SDL_SetTextureColorMod(texture, 255, 0, 0);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }

                                if (dstRect.x + 5 < characterDestRect.x + 16 && dstRect.y + 5 < characterDestRect.y + 54)
                                {
                                    velNeg = 0;
                                    velUp = 10;
                                    isJumping = true;
                                    SDL_SetTextureColorMod(texture, 0, 0, 255);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }

                                if (dstRect.y + 5 < characterDestRect.y + 27)
                                {
                                    velUp = 0;
                                    // isJumping = false;
                                    SDL_SetTextureColorMod(texture, 255, 255, 0);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                                }
                            }
                        }
                    }
                }
            }

            if (isJumping)
            {
                float dt = 1.0 / 60;
                // cameraRect.y -= velUp * dt;
                jumpDuration -= dt;
                velUp -= dt;
                cameraRect.y += (grav - velUp);
                // velUp -= 0.05;
                if (velUp <= 0)
                {
                    velUp = 0;
                }

                if (jumpDuration <= 0)
                {
                    isJumping = false;
                    jumpDuration = 0.5;
                }
            }
            else
            {
                cameraRect.y += grav;
            }

            cameraRect.x += velPos + velNeg;

            rect.x = renderRect.w / 2 - 5 / 2;
            rect.y = renderRect.h / 2 - 5 / 2;
            characterDestRect.x = renderRect.w / 2 - 5 / 2;
            characterDestRect.y = renderRect.h / 2 - 5 / 2;

            SDL_SetTextureColorMod(texture, 255, 255, 255);

            SDL_RenderCopyEx(renderer, spriteSheetTexture, &sprites[currentFrame], &characterDestRect, 0, NULL, flip);

            // Update the current frame of the animation based on elapsed time
            if (playAnim)
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
protected:
};
#endif // RENDERER_H