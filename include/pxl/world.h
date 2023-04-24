#ifndef WORLD_H
#define WORLD_H
#include<pxl/renderer.h>
/**
 * @brief
 * Game World Logic
 */
class World
{
public:
    /**
     * @brief Construct a New World orde....uurh Object :)
     *
     */
    World(Renderer &renderer) : renderer_(renderer) {}

protected:
private:
    Renderer& renderer_;
};

#endif // WORLD_H
