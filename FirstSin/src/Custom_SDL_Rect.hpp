#ifndef SDL_RECT_H
#define SDL_RECT_H

#include <SDL.h>

// only checks the x,y
inline bool operator==(const SDL_Rect &lhs, const SDL_Rect &rhs) {
    if (lhs.x != rhs.x) {
        return false;
    }
    if (lhs.y != rhs.y) { return false; }
    return true;
}


#endif
