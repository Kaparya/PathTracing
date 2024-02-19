
constexpr int SobolMatrixSize = 52;
extern const uint32_t SobolMatrices32[53'248];

float UintToFloat(uint32_t value) {
    return std::min(1 - 1e-10f, value * 0x1p-32f);
}

static float SobolRand(uint32_t index, uint32_t dimension) {

    uint32_t value = 0;
    for (uint32_t i = dimension * SobolMatrixSize; index != 0; index >>= 1, ++i) {
        if (index & 1) {
            value ^= SobolMatrices32[i];
        }
    }

    return UintToFloat(value);
}