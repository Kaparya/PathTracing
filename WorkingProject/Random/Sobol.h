
static constexpr int NumSobolDimensions = 1024;
static constexpr int SobolMatrixSize = 52;
extern const uint32_t SobolMatrices32[NumSobolDimensions *
                                      SobolMatrixSize];

static inline float UintToFloat(uint32_t value) {
    return std::min(1 - 1e-10f, value * 0x1p-32f);
}

inline uint32_t OrdinarySobol(uint32_t index, uint32_t dimension,
                              uint32_t scramble = 0) {
    uint32_t value = scramble;
    for (uint32_t i = dimension * SobolMatrixSize; index != 0; index >>= 1, ++i) {
        if (index & 1) {
            value ^= SobolMatrices32[i];
        }
    }
    return value;
}


inline float SobolRand(uint32_t index, uint32_t dimension,
                       uint32_t scramble = 0) {
    return UintToFloat(OrdinarySobol(index, dimension, scramble));
}

inline float SobolOwenScrambling(uint32_t index, uint32_t dimension,
                            uint32_t scramble) {

    uint32_t value = OrdinarySobol(index, dimension, scramble);

    for (size_t i = 0; i < 16; ++i) {
        uint32_t mask = (1 << (31 - i)) - 1;
        uint32_t hash;
        {
            hash = value & (!mask);
            uint32_t seed = scramble + i;
            std::vector<uint32_t> perms = {0x29aaaaa7, 0x736caf6f, 0x54aad35b, 0x2ab35aaa, 0x161fe91a, 0xf91a7b2c, 0x19460471, 0xad16072f};
            for (size_t j = 0; j < 10; ++j) {
                hash = hash * perms[j % perms.size()];
                hash ^= hash >> 16;
                hash ^= seed;
            }
        }

        value ^= hash & mask;
    }

    return UintToFloat(value);
}