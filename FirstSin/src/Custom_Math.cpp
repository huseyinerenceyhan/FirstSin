#include <random>
#include <ctime>
#include <stdexcept>

#include "Custom_Math.hpp"

int getRandomIntWithBounds(int min, int max) {
    if (min > max) {
        throw std::invalid_argument("Min cannot be greater than max.");
    }

    static std::random_device rd;
    static std::mt19937 engine(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(engine);
}
