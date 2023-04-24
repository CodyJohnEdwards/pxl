#ifndef CORE_H
#define CORE_H
#include <pxl/logic.h>
/**
 * @brief
 * Applied Logic & Physics
 */
class Core : public Logic
{
private:

public:
    Core()
    {
      
    }
    double **generateTerrain()
    {
        // Allocate memory for the gradients and noise map
        double *gradients[MAX_HEIGHT];
        double **noiseMap = new double *[MAX_HEIGHT];
        for (int y = 0; y < MAX_HEIGHT; y++)
        {
            gradients[y] = new double[MAX_WIDTH * 2];
            noiseMap[y] = new double[MAX_WIDTH];
        }

        // Generate the gradients
        Logic::generateGradients(MAX_WIDTH, MAX_HEIGHT, gradients);

        // Generate the noise map
        Logic::generatePerlinNoise(MAX_WIDTH, MAX_HEIGHT, gradients, noiseMap);

        // Deallocate memory for the gradients
        for (int y = 0; y < MAX_HEIGHT; y++)
        {
            delete[] gradients[y];
        }

        // Return the noise map
        return noiseMap;
    }
};

#endif // CORE_H
