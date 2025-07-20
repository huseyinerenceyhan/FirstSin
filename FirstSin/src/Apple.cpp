#include <SDL.h>
#include <stdexcept>

#include "Apple.hpp"
#include "Snake.hpp"
#include "Grid.hpp"

Apple::Apple(const SpriteSheet &appleSprite, Grid *grid)
    : spritesheet(appleSprite) {
    position = {(grid->tileSize * 7), (grid->tileSize * 7), renderSize, renderSize};
}

void Apple::render(SDL_Renderer *gRenderer) {
    if (!gRenderer) {
        throw std::invalid_argument("*gRenderer is null in Apple::render.");
    }
    SDL_Rect renderPos = centerRenderPosition();
    SDL_RenderCopy(gRenderer, spritesheet.sprite, nullptr, &renderPos);
}

SDL_Rect Apple::centerRenderPosition() {
    SDL_Rect adjustedPosition = position;
    int adjustAmount = (renderSize) / 2;
    adjustedPosition.x += adjustAmount;
    adjustedPosition.y += adjustAmount;
    return adjustedPosition;
}
