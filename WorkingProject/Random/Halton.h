#pragma once

#include "Scrambling.h"

inline float HaltonRand(uint32_t index, const uint32_t base,
                        uint32_t scramble, int scrambling_type) {
    const float invBase = (float) 1 / (float) base;
    uint32_t value = 0;
    float invBaseN = 1;

    while (index) {
        uint32_t next = index / base;
        uint32_t digit = index - next * base;
        value = value * base + digit;
        invBaseN *= invBase;
        index = next;
    }

    if (scrambling_type == None) {
        return std::min(value * invBaseN, 1 - 1e-10f);
    }
    value = (value << 8) + value;
    value = (value << 16) + value;
    switch (scrambling_type) {
        case 0:
            value ^= scramble;
            break;
        case 1:
            value = OwenScrambling<1>(value, scramble);
            break;
        case 2:
            value = OwenScrambling<2>(value, scramble);
            break;
        case 3:
            value = OwenScrambling<3>(value, scramble);
            break;
        case 4:
            value = OwenScrambling<4>(value, scramble);
            break;
        default:
            break;
    }

    return UintToFloat(value);
}
