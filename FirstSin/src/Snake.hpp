#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SDL.h>

#include "SpriteSheet.hpp"


enum SpecialCharacters {
    DEVIL,
    ADAM,
    EVE,
    COUNT
};

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

struct SnakeNode {
    SnakeNode *nextNode;
    SnakeNode *prevNode;
    SDL_Rect animation{};
    SDL_Rect position{};
    Uint64 startTime;

    SnakeNode(SDL_Rect anim, SnakeNode *next, SnakeNode *prev);

    void setNewPosition();
};

struct Snake {
    SnakeNode *headNode;
    SnakeNode *tailNode;
    const int stepSize;
    const int renderSize;
    const int animationRate = 10;
    const int animationLength = 3;
    SpriteSheet spriteSheet;
    Direction direction = RIGHT;
    bool hasStepped = false;

    SDL_Rect playAreaRect;

    Snake(const SpriteSheet &sprite, int step, SDL_Rect headPosition, SDL_Rect playArea);

    ~Snake();

    void render(SDL_Renderer *gRenderer);


    void grow();

    void moveHeadToDirection();

    void moveBody();

    void move();

    void checkBoundsY();

    void checkBoundsX();

    void handleEvent(SDL_Event &e);

    SDL_Rect getNextAnimationFrame(SDL_Rect r);
};


#endif
