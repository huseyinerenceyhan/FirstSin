#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Snake.hpp"
#include "Apple.hpp"
#include "Grid.hpp"


struct Game {
    // never free the renderer
    SDL_Renderer *renderer;
    Snake *snake;
    Grid *grid;
    Apple *apple;
    Timer *moveTimer;
    Timer *respawnTimer;
    SpriteSheet *snakeSprite;
    SpriteSheet *appleSprite;
    Mix_Chunk *pickUp;
    TTF_Font *font;
    SDL_Texture *fontTexture;
    SDL_Texture *scoreTexture;
    SDL_Rect fontRect;
    SDL_Rect scoreRect;

    const int scoreX = 750;
    const int scoreY = 150;
    int score;
    std::string scoreString;

    const int MOVE_IN_MILISECONDS = 250;
    const int RESPAWN_TIME = 600;


    bool canRespawn = false;


    Game(SDL_Renderer *gRenderer);

    ~Game();

    void render();

    void checkCollisionWithApple();

    void move();

    void checkDeathCondition();

    void respawnApple(int timePassed);

    void clearPointers();


    void initElements();

    void initFont();

    void renderScore();

    void restart();
};

#endif
