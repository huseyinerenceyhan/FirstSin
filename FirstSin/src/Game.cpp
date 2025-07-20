#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

#include "Snake.hpp"
#include "Timer.hpp"
#include "Apple.hpp"
#include "Grid.hpp"
#include "Game.hpp"
#include "Custom_SDL_Rect.hpp"


Game::Game(SDL_Renderer *gRenderer) {
    renderer = gRenderer;
    initElements();
}

Game::~Game() {
    clearPointers();
}

void Game::render() {
    grid->render(renderer);
    apple->render(renderer);
    snake->render(renderer);
    renderScore();
}

void Game::checkCollisionWithApple() {
    if (apple->position == snake->headNode->position) {
        Mix_PlayChannel(-1, pickUp, 0);
        score++;
        scoreString = std::to_string(score);
        snake->grow();
        //move the apple out of vision
        apple->position.x += 1000;
        respawnTimer->start();
        canRespawn = true;
    }
}

void Game::move() {
    grid->updateTileEmptiness(snake->tailNode, true);
    snake->move();
    checkDeathCondition();
    grid->updateTileEmptiness(snake->headNode, false);
    if (!canRespawn) {
        checkCollisionWithApple();
    } else {
        respawnApple(respawnTimer->getTicks());
    }
}

void Game::checkDeathCondition() {
    SnakeNode *node = snake->headNode->prevNode;
    while (node) {
        if (node->position == snake->headNode->position) {
            restart();
        }
        node = node->prevNode;
    }
}

void Game::respawnApple(int timePassed) {
    if (timePassed >= RESPAWN_TIME) {
        grid->updateEmptyTilesVector();
        apple->position = grid->getRandomEmptyTile();
        apple->position.w = apple->renderSize;
        apple->position.h = apple->renderSize;
        //  grid->print();
        canRespawn = false;
        respawnTimer->start();
    }
}

void Game::clearPointers() {
    delete snake;
    snake = nullptr;
    delete grid;
    grid = nullptr;
    delete apple;
    apple = nullptr;
    delete moveTimer;
    moveTimer = nullptr;
    delete respawnTimer;
    respawnTimer = nullptr;
    delete snakeSprite;
    snakeSprite = nullptr;
    delete appleSprite;
    appleSprite = nullptr;
    if (pickUp) {
        Mix_FreeChunk(pickUp);
        pickUp = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (fontTexture) {
        SDL_DestroyTexture(fontTexture);
        fontTexture = nullptr;
    }
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }
}

void Game::initElements() {
    score = 0;
    scoreString = "0";
    int stepSize = 48;
    SDL_Rect playAreaRect = {0, 0, 720, 720};
    grid = new Grid(playAreaRect, stepSize);
    SDL_Rect spriteRect = {stepSize * 3, 0, stepSize, stepSize};
    snakeSprite = new SpriteSheet(renderer, "assets/walk_animations.png", 64, 66);
    appleSprite = new SpriteSheet(renderer, "assets/apple.png", 64, 64);
    snake = new Snake(*snakeSprite, stepSize, spriteRect, playAreaRect);
    grid->markSnakeNodesAtStart(snake);
    apple = new Apple(*appleSprite, grid);
    moveTimer = new Timer();
    respawnTimer = new Timer();
    pickUp = Mix_LoadWAV("assets/pickup.wav");
    initFont();
    moveTimer->start();
}

void Game::initFont() {
    font = TTF_OpenFont("assets/font.otf", 40);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "SCORE", SDL_Color{0xFF, 0xFF, 0xFF});
    fontTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    fontRect.x = scoreX;
    fontRect.y = scoreY;
    fontRect.w = textSurface->w;
    fontRect.h = textSurface->h;
    // for centering the text
    scoreRect.x = scoreX + 50;
    scoreRect.y = scoreY + fontRect.h + 20;
    SDL_FreeSurface(textSurface);
}

void Game::renderScore() {
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreString.c_str(), SDL_Color{0xFF, 0xFF, 0xFF});
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    scoreRect.w = scoreSurface->w;
    scoreRect.h = scoreSurface->h;
    SDL_FreeSurface(scoreSurface);
    SDL_RenderCopy(renderer, fontTexture,NULL, &fontRect);
    SDL_RenderCopy(renderer, scoreTexture,NULL, &scoreRect);
}


void Game::restart() {
    clearPointers();
    initElements();
    canRespawn = false;
}
