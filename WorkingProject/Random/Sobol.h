#ifndef SOBOL_H
#define SOBOL_H

static constexpr int NumSobolDimensions = 1024;
static constexpr int SobolMatrixSize = 52;
extern const uint32_t SobolMatrices32[NumSobolDimensions *
                                      SobolMatrixSize];

static float SobolRand(uint32_t index, uint32_t dimension) {

    uint32_t value = 0;
    for (uint32_t i = dimension * SobolMatrixSize; index != 0; index >>= 1, ++i) {
        if (index & 1) {
            value ^= SobolMatrices32[i];
        }
    }

    return std::min(value * 0x1p-32f, 1.f - 1e-6f);
}

#endif
