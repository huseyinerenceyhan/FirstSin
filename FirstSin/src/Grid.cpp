#include <SDL.h>
#include <stdexcept>

#include "Grid.hpp"
#include "Custom_Math.hpp"

Tile::Tile(SDL_Rect r): rect(r) {
}


Grid::Grid(const SDL_Rect playRect, int sizeOfTile)
    : tileSize(sizeOfTile), gridX(playRect.x), gridY(playRect.y), gridWidth(playRect.w), gridHeight(playRect.h) {
    if (sizeOfTile <= 0) {
        printf("tile size: %d\n", sizeOfTile);
        throw std::invalid_argument("tileSize is not positive in Grid Constructor.");
    }

    rowCount = playRect.h / sizeOfTile;
    columnCount = playRect.w / sizeOfTile;
    playArea.resize(rowCount);
    for (int i = 0; i < playArea.size(); i++) {
        playArea.at(i).resize(columnCount);
    }
    SDL_Rect r;
    for (int x = 0; x < columnCount; x++) {
        for (int y = 0; y < rowCount; y++) {
            r = {playRect.x + (x * sizeOfTile), playRect.y + (y * sizeOfTile), sizeOfTile, sizeOfTile};
            // printf("x: %d  y: %d\n", x, y);
            playArea[y][x] = Tile(r);
        }
    }
}


void Grid::markSnakeNodesAtStart(Snake *snake) {
    if (!snake) {
        throw std::invalid_argument("*snake is null in Grid::markSnakeNodesAtStart.");
    }
    SnakeNode *node = snake->tailNode;
    int column;
    int row;
    while (node) {
        column = node->position.x / tileSize;
        row = node->position.y / tileSize;
        playArea[row][column].empty = false;
        node = node->nextNode;
    }
}

void Grid::updateTileEmptiness(SnakeNode *node, bool empty) {
    if (!node) {
        throw std::invalid_argument("*node is null in Grid::updateTileEmptiness.");
    }
    int column = node->position.x / tileSize;
    int row = node->position.y / tileSize;
    playArea[row][column].empty = empty;
}


void Grid::updateEmptyTilesVector() {
    emptyTiles.clear();

    for (int x = 0; x < columnCount; x++) {
        for (int y = 0; y < rowCount; y++) {
            Tile tile = playArea[y][x];
            if (tile.empty) {
                emptyTiles.push_back(tile);
            }
        }
    }
    // printf("EMPTY TILES START\n");
    // for (Tile t: emptyTiles) {
    //     printf("row: %d, column:%d\n", t.rect.x / 48, t.rect.y / 48);
    // }
    // printf("EMPTY TILES SIZE: %d\n", emptyTiles.size());
}

void Grid::render(SDL_Renderer *gRenderer) {
    if (!gRenderer) {
        throw std::invalid_argument("*gRenderer is null in Grid::render.");
    }
    SDL_SetRenderDrawColor(gRenderer, 0x3A, 0x3A, 0xFF, 0xFF);

    for (int y = 0; y <= rowCount; y++) {
        SDL_RenderDrawLine(gRenderer, 0, tileSize * y, gridWidth, tileSize * y);
    }
    for (int x = 0; x <= columnCount; x++) {
        SDL_RenderDrawLine(gRenderer, tileSize * x, 0, tileSize * x, gridHeight);
    }
}

void Grid::print() {
    for (int x = 0; x < columnCount; x++) {
        for (int y = 0; y < rowCount; y++) {
            // switched x and y to match the display
            printf("%hhd    ", playArea[x][y].empty);
        }
        printf("\n");
    }
    printf("NEW FRAME\n\n\n");
}


SDL_Rect Grid::getRandomEmptyTile() {
    int random = getRandomIntWithBounds(0, emptyTiles.size() - 1);
    return emptyTiles.at(random).rect;
}
