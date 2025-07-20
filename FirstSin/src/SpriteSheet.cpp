#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <ctime>

#include "SpriteSheet.hpp"


SpriteSheet::SpriteSheet(SDL_Renderer *gRenderer, const std::string &file, const int frameW, const int frameH)
    : sprite(nullptr), frameWidth(frameW), frameHeight(frameH) {
    if (frameW <= 0 || frameH <= 0) {
        throw std::invalid_argument("frameW or frameH is not positive on SpriteSheet Constructor.");
    }
    if (!gRenderer) {
        throw std::invalid_argument("*gRenderer is null on SpriteSheet Constructor.");
    }

    if (!loadSpriteSheet(file, gRenderer)) {
        throw std::invalid_argument("&file is null on SpriteSheet Constructor.");
    }

    fillAnimations();
}

bool SpriteSheet::loadSpriteSheet(const std::string &file, SDL_Renderer *gRenderer) {
    sprite = IMG_LoadTexture(gRenderer, file.c_str());
    if (!sprite) {
        printf("Failed to load texture: %s\n%s\n", SDL_GetError(), file.c_str());
        return false;
    }


    SDL_QueryTexture(sprite, NULL, NULL, &sheetWidth, &sheetHeight);

    columnCount = sheetWidth / frameWidth;
    rowCount = sheetHeight / frameHeight;
    return true;
}

void SpriteSheet::fillAnimations() {
    int frameCount = rowCount;
    animations.resize(frameCount);
    for (int i = 0; i < frameCount; i++) {
        animations[i].x = 0;
        animations[i].y = i * frameHeight;
        animations[i].w = frameWidth * columnCount;
        animations[i].h = frameHeight;
    }
}


SDL_Rect SpriteSheet::getRow(const int row) const {
    if (row < 0 || row > animations.size()) {
        printf(" for row: %d ", row);
        throw std::out_of_range("row out of bounds in SDL_Rect SpriteSheet::getRow \n");
    }

    return animations[row];
}

SpriteSheet::~SpriteSheet() {
    if (sprite) {
        SDL_DestroyTexture(sprite);
    }
}
