#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <SDL.h>
#include <string>
#include <vector>


struct SpriteSheet {
    SDL_Texture *sprite;
    std::vector<SDL_Rect> animations;
    int frameWidth, frameHeight;
    int sheetWidth = 0, sheetHeight = 0;
    int columnCount = 0, rowCount = 0;

    SpriteSheet(SDL_Renderer *gRenderer, const std::string &file, int frameW, int frameH);

    bool loadSpriteSheet(const std::string &file, SDL_Renderer *gRenderer);

    void fillAnimations();

    SDL_Rect getRow(int row) const;

    ~SpriteSheet();
};


#endif
