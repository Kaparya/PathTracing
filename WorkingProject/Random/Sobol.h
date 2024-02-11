#pragma once

#include "Scrambling.h"

static constexpr int NumSobolDimensions = 1024;
static constexpr int SobolMatrixSize = 52;
extern const uint32_t SobolMatrices32[NumSobolDimensions *
                                      SobolMatrixSize];

static float SobolRand(uint32_t index, uint32_t dimension,
                       uint32_t scramble, int scrambling_type) {

    uint32_t value = 0;
    for (uint32_t i = dimension * SobolMatrixSize; index != 0; index >>= 1, ++i) {
        if (index & 1) {
            value ^= SobolMatrices32[i];
        }
    }

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
