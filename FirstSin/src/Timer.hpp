#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL.h>


struct Timer {
    Timer();

    void start();

    Uint64 getTicks() const;

    Uint64 startTicks;
};


#endif
