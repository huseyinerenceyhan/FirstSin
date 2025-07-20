#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "Snake.hpp"


struct Tile {
    SDL_Rect rect;
    bool empty = true;

    Tile(SDL_Rect r);

    // not a valid constructor. used for playArea resizing
    Tile() = default;
};

struct Grid {
    int tileSize;
    int gridX, gridY;
    int gridWidth = 0, gridHeight = 0;
    int columnCount = 0, rowCount = 0;
    std::vector<std::vector<Tile> > playArea;
    std::vector<Tile> emptyTiles;

    Grid(SDL_Rect playRect, int sizeOfTile);

    void render(SDL_Renderer *gRenderer);

    void markSnakeNodesAtStart(Snake *snake);


    void updateTileEmptiness(SnakeNode *node, bool empty);

    void updateEmptyTilesVector();

    void print();

    SDL_Rect getRandomEmptyTile();
};


#endif
