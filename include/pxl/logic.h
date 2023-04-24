#ifndef LOGIC_H
#define LOGIC_H
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
/**
 * @brief
 * SDL2 Wrapper + Game Logic
 */
class Logic
{
public:
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    const int MAX_WIDTH = 5000;
    const int OCTAVES = 6;
    const int MAX_HEIGHT = 1000;
    Logic()
    {
       
    }
    void init(){
         // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError());
            SDL_Quit();
        }
        // Create a window and renderer
        window = SDL_CreateWindow("SDL 2 Program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
        if (!window)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
            SDL_Quit();
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer: %s", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
    /**
     * @brief
     * Calculate the distance between two points
     * @param p1
     * @param p2
     * @return float
     */
    float distance(SDL_Point p1, SDL_Point p2)
    {
        return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    }

protected:

    /**
     * @brief
     * Computes the dot product of the distance and gradient vectors
     * @param ix
     * @param iy
     * @param x
     * @param y
     * @param gradients
     * @return double
     */

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

    /**
     * @brief
     * Computes the Perlin noise value at a given point
     * @param x
     * @param y
     * @param gradients
     * @return double
     */
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

    /**
     * @brief
     * Generates a 2D Perlin noise map with a given width and height
     * @param width
     * @param height
     * @param gradients
     * @param noiseMap
     */
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

    /**
     * @brief
     * Generates random gradient vectors for each grid point
     * @param width
     * @param height
     * @param gradients
     */
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

private:
};

#endif // LOGIC_H
