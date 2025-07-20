#include "Timer.hpp"


Timer::Timer() {
    startTicks = 0;
}

void Timer::start() {
    startTicks = SDL_GetTicks64();
}


Uint64 Timer::getTicks() const {
    Uint64 time = SDL_GetTicks64() - startTicks;
    return time;
}
