
static constexpr int NumSobolDimensions = 1024;
static constexpr int SobolMatrixSize = 52;
extern const uint32_t SobolMatrices32[NumSobolDimensions *
                                      SobolMatrixSize];

inline float SobolRand(uint32_t index, uint32_t dimension,
                       uint32_t scramble = 0) {
    uint32_t v = scramble;
    for (uint32_t i = dimension * SobolMatrixSize; index != 0; index >>= 1, ++i) {
        if (index & 1) {
            v ^= SobolMatrices32[i];
        }
    }
    return v * 0x1p-32f; /* 1/2^32 */
}