#include <stdexcept>

#include "Snake.hpp"
#include "SpriteSheet.hpp"
#include "Custom_SDL_Rect.hpp"
#include "Custom_Math.hpp"


SnakeNode::SnakeNode(const SDL_Rect anim, SnakeNode *next, SnakeNode *prev) {
    animation = anim;
    startTime = SDL_GetTicks64();
    nextNode = next;
    prevNode = prev;
}

void SnakeNode::setNewPosition() {
    if (nextNode) {
        position = nextNode->position;
    }
}


Snake::Snake(const SpriteSheet &sprite, int step, const SDL_Rect headPosition, const SDL_Rect playArea)
    : spriteSheet(sprite), stepSize(step), renderSize(step), playAreaRect(playArea) {
    headNode = new SnakeNode(spriteSheet.getRow(DEVIL), nullptr, nullptr);
    headNode->position = headPosition;


    SnakeNode *middleNode = new SnakeNode(spriteSheet.getRow(EVE), headNode, nullptr);
    headNode->prevNode = middleNode;
    middleNode->position = headNode->position;
    middleNode->position.x -= headNode->position.w;


    tailNode = new SnakeNode(spriteSheet.getRow(ADAM), middleNode, nullptr);
    middleNode->prevNode = tailNode;
    tailNode->position = middleNode->position;
    tailNode->position.x -= middleNode->position.w;
}

Snake::~Snake() {
    SnakeNode *current = headNode;
    while (current != nullptr) {
        SnakeNode *next = current->nextNode;
        delete current;
        current = next;
    }
    headNode = nullptr;
    tailNode = nullptr;
}

void Snake::grow() {
    SnakeNode *newNode = new SnakeNode(tailNode->animation, tailNode, nullptr);
    newNode->position = {tailNode->position.x, tailNode->position.y, renderSize, renderSize};
    int random = getRandomIntWithBounds(SpecialCharacters(COUNT), spriteSheet.animations.size() - 1);
    // printf("RANDOM: %d\n", random);
    tailNode->nextNode->animation = spriteSheet.animations[random];
    tailNode->prevNode = newNode;
    tailNode = newNode;
    tailNode->nextNode->animation = spriteSheet.animations[SpecialCharacters(EVE)];
}


void Snake::handleEvent(SDL_Event &e) {
    if (!hasStepped) { return; }
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (direction == DOWN) { break; }

                direction = UP;
                hasStepped = false;
                break;
            case SDLK_DOWN:

                if (direction == UP) { break; }
                direction = DOWN;
                hasStepped = false;
                break;
            case SDLK_LEFT:

                if (direction == RIGHT) { break; }
                direction = LEFT;
                hasStepped = false;
                break;
            case SDLK_RIGHT:

                if (direction == LEFT) { break; }
                direction = RIGHT;
                hasStepped = false;
                break;
        }
    }
}


void Snake::moveHeadToDirection() {
    switch (direction) {
        case UP:
            headNode->position.y -= stepSize;

            checkBoundsY();
            //  printf("MOVING UP\n");
            break;
        case DOWN:
            headNode->position.y += stepSize;

            checkBoundsY();
            //   printf("MOVING DOWN\n");
            break;
        case LEFT:
            headNode->position.x -= stepSize;

            checkBoundsX();
            //   printf("MOVING LEFT\n");
            break;
        case RIGHT:
            headNode->position.x += stepSize;

            checkBoundsX();
            //   printf("MOVING RIGHT\n");
            break;
    }
}

void Snake::moveBody() {
    hasStepped = true;
    SnakeNode *node = tailNode;
    while (node) {
        node->setNewPosition();
        node = node->nextNode;
    }
}

void Snake::move() {
    moveBody();
    moveHeadToDirection();
}

void Snake::checkBoundsY() {
    if (headNode->position.y >= playAreaRect.h) {
        headNode->position.y = playAreaRect.y;
        return;
    }
    if (headNode->position.y < 0) {
        headNode->position.y = playAreaRect.h - stepSize;
    }
}


void Snake::checkBoundsX() {
    if (headNode->position.x >= playAreaRect.w) {
        headNode->position.x = playAreaRect.x;
        return;
    }
    if (headNode->position.x < 0) {
        headNode->position.x = playAreaRect.w - stepSize;
    }
}


void Snake::render(SDL_Renderer *gRenderer) {
    if (!gRenderer) {
        throw std::invalid_argument("*gRenderer is null in Snake::render.");
    }
    SnakeNode *node;
    node = tailNode;
    SDL_Rect rect;
    while (node) {
        rect = getNextAnimationFrame(node->animation);

        SDL_RenderCopy(gRenderer, spriteSheet.sprite, &rect, &(node->position));
        node = node->nextNode;
    }
}

SDL_Rect Snake::getNextAnimationFrame(SDL_Rect rect) {
    /*
     * Thank You, Miguel Grinberg
     * https://stackoverflow.com/questions/7425890/c-sdl-animation-speed
     */
    int draw = (SDL_GetTicks64() * animationRate / 1000) % animationLength;
    rect.x = draw * spriteSheet.frameWidth;
    rect.w = 64;
    return rect;
}
