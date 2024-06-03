#ifndef STANDARD_H
#define STANDARD_H

#include <iostream>
#include <random>

float StandardRand() {
    // Discovered in 1969 by Lewis, Goodman and Miller, adopted as "Minimal standard" in 1988 by Park and Miller
    static std::linear_congruential_engine<uint32_t, 16807, 0, 2147483647> engine;
    static std::uniform_real_distribution<float> distribution(0, 1);
    return distribution(engine);
}

#endif