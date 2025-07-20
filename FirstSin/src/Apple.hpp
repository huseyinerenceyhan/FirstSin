#ifndef APPLE_HPP
#define APPLE_HPP

#include <SDL.h>

#include "SpriteSheet.hpp"
#include "Grid.hpp"


struct Apple {
    SpriteSheet spritesheet;
    SDL_Rect position{};
    int renderSize = 24;

    Apple(const SpriteSheet &appleSprite, Grid *grid);

    void render(SDL_Renderer *gRenderer);

    SDL_Rect centerRenderPosition();
};


#endif
