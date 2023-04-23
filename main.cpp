#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

const int MAX_WIDTH = 5000;
const int MAX_HEIGHT = 1000;
const int OCTAVES = 6;

// Computes the dot product of the distance and gradient vectors
double dotGridGradient(int ix, int iy, double x, double y, double *gradients[])
{
  // Compute the distance vector
  double dx = x - (double)ix;
  double dy = y - (double)iy;

  // Compute the gradient vector
  double gradientX = gradients[iy][ix * 2];
  double gradientY = gradients[iy][ix * 2 + 1];

  // Compute the dot product
  return (dx * gradientX + dy * gradientY);
}

// Computes the Perlin noise value at a given point
double perlin(double x, double y, double *gradients[])
{
  // Determine the grid cell coordinates
  int x0 = (int)x;
  int x1 = x0 + 1;
  int y0 = (int)y;
  int y1 = y0 + 1;

  // Determine the fractional part of the coordinates
  double fracX = x - (double)x0;
  double fracY = y - (double)y0;

  // Get the dot products of the four corners of the cell
  double dot00 = dotGridGradient(x0, y0, x, y, gradients);
  double dot01 = dotGridGradient(x0, y1, x, y, gradients);
  double dot10 = dotGridGradient(x1, y0, x, y, gradients);
  double dot11 = dotGridGradient(x1, y1, x, y, gradients);

  // Interpolate the dot products using a smoothstep function
  double interpX0 = dot00 * (1 - fracX) + dot10 * fracX;
  double interpX1 = dot01 * (1 - fracX) + dot11 * fracX;
  double interpY = interpX0 * (1 - fracY) + interpX1 * fracY;

  return interpY;
}

// Generates a 2D Perlin noise map with a given width and height
void generatePerlinNoise(int width, int height, double *gradients[], double *noiseMap[])
{
  // Create a random number generator
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  // Generate the noise map
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      double amplitude = 2.5;
      double frequency = 50.0;
      double noise = 0.0;

      for (int octave = 0; octave < OCTAVES; octave++)
      {
        double sampleX = x / frequency;
        double sampleY = y / frequency;
        double value = perlin(sampleX, sampleY, gradients);
        noise += value * amplitude;
        amplitude *= 0.5;
        frequency *= 2.0;
      }

      noiseMap[y][x] = noise;
    }
  }
}

// Generates random gradient vectors for each grid point
void generateGradients(int width, int height, double *gradients[])
{
  // Create a random number generator
  std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);

  // Generate the gradient vectors
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      double angle = distribution(generator) * M_PI;
      gradients[y][x * 2] = std::cos(angle);
      gradients[y][x * 2 + 1] = std::sin(angle);
    }
  }
}

// Calculate the distance between two points
float distance(SDL_Point p1, SDL_Point p2)
{
  return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

int main(int argc, char *argv[])
{

  //--------------terrain

  // Allocate memory for the gradients and noise map
  double *gradients[MAX_HEIGHT];
  double *noiseMap[MAX_HEIGHT];
  for (int y = 0; y < MAX_HEIGHT; y++)
  {
    gradients[y] = new double[MAX_WIDTH * 2];
    noiseMap[y] = new double[MAX_WIDTH];
  }

  // Generate the gradients
  generateGradients(MAX_WIDTH, MAX_HEIGHT, gradients);

  // Generate the noise map
  generatePerlinNoise(MAX_WIDTH, MAX_HEIGHT, gradients, noiseMap);

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
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

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  // Create a window and renderer
  window = SDL_CreateWindow("SDL 2 Program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
  if (!window)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Load the texture image
  SDL_Texture *texture = NULL;
  std::string textureFile = "text.png";
  SDL_Surface *surface = SDL_LoadBMP("./texture.bmp");
  SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface);

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if (!renderer)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

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
        rect.y
    };

    // Convert camera position to render coordinates
    SDL_Rect renderRect = {
        cameraRect.x,
        cameraRect.y,
        cameraRect.w,
        cameraRect.h
    };

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
          if(!isJumping){
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
        switch (event.key.keysym.sym) {
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
    const Uint8* keyboard_state_array;
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

    for (int row = 100; row < MAX_HEIGHT; row++)
    {
      for (int tile = 0; tile < MAX_WIDTH; tile++)
      {

        int tileX = tile * 5;
        int tileY = row * 5;

        // check if tile is within renderRect
        if (tileX >= renderRect.x && tileX < renderRect.x + renderRect.w && tileY >= renderRect.y && tileY < renderRect.y + renderRect.h)
        {

          if (noiseMap[row][tile] > 0)
          {

            SDL_Rect dstRect = {
                tileX - renderRect.x,
                tileY - renderRect.y,
                5,
                5};

            SDL_Point rect2Pos = {
                dstRect.x,
                dstRect.y};

            float d = distance(playerPos, rect2Pos);
            float brightness = 1.0 - d / 100;
            brightness = fmaxf(brightness, 0.0); // clamp @ 1 || 0

            SDL_SetTextureColorMod(texture, brightness * 255, brightness * 255, brightness * 255);
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            if(SDL_HasIntersection(&dstRect,&characterDestRect)){
               if(dstRect.y > characterDestRect.y - 27){
                grav = 0;
                SDL_SetTextureColorMod(texture, 0,255,0);
                SDL_RenderCopy(renderer, texture, NULL, &dstRect);
              }
              if(dstRect.x + 5 > characterDestRect.x + 16 && dstRect.y + 5 < characterDestRect.y + 54){
                velPos = 0;
                velUp = 10;
                isJumping = true;
                SDL_SetTextureColorMod(texture, 255,0,0);
                SDL_RenderCopy(renderer, texture, NULL, &dstRect);
              }

            

             

                if(dstRect.x + 5 < characterDestRect.x + 16 && dstRect.y + 5 < characterDestRect.y + 54){
                velNeg = 0;
                velUp = 10;
                isJumping = true;
                SDL_SetTextureColorMod(texture, 0,0,255);
                SDL_RenderCopy(renderer, texture, NULL, &dstRect);
              }

              if(dstRect.y + 5 < characterDestRect.y + 27){
                velUp = 0;
                // isJumping = false;
                SDL_SetTextureColorMod(texture, 255,255,0);
                SDL_RenderCopy(renderer, texture, NULL, &dstRect);
              }

            }
          }
        }
      }
    }
    
    if(isJumping){
      float dt = 1.0 / 60;
      // cameraRect.y -= velUp * dt;
      jumpDuration -= dt;
      velUp -= dt;
      cameraRect.y += (grav - velUp);
      // velUp -= 0.05;
      if(velUp <= 0){
        velUp = 0;
      }

      if(jumpDuration <= 0){
        isJumping = false;
        jumpDuration = 0.5;
      }
    }else{
       cameraRect.y += grav;
    }


    cameraRect.x += velPos + velNeg;

    rect.x = renderRect.w / 2 - 5 / 2;
    rect.y = renderRect.h / 2 - 5 / 2;
    characterDestRect.x = renderRect.w / 2 - 5 / 2;
    characterDestRect.y = renderRect.h / 2 - 5 / 2;

    SDL_SetTextureColorMod(texture, 255, 255, 255);



    SDL_RenderCopyEx(renderer, spriteSheetTexture, &sprites[currentFrame], &characterDestRect, 0,NULL,flip);

    // Update the current frame of the animation based on elapsed time
    if(playAnim){

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime >= frameTime)
    {
      lastFrameTime = currentTime;
      currentFrame = (currentFrame + 1) % 4; // There are 4 frames in this animation
    }
    }else{
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
  for (int y = 0; y < MAX_HEIGHT; y++)
  {
    delete[] gradients[y];
    delete[] noiseMap[y];
  }

  return 0;
}