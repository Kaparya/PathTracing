#pragma once

namespace {
    inline uint32_t ReverseBits(uint32_t value) {

        value = (value << 16) | (value >> 16);
        value = ((value & 0x00ff00ff) << 8) | ((value & 0xff00ff00) >> 8);
        value = ((value & 0x0f0f0f0f) << 4) | ((value & 0xf0f0f0f0) >> 4);
        value = ((value & 0x33333333) << 2) | ((value & 0xcccccccc) >> 2);
        value = ((value & 0x55555555) << 1) | ((value & 0xaaaaaaaa) >> 1);
        return value;
    }

    inline uint32_t lk_hash(uint32_t value, uint32_t seed) {

        value += seed;
        value ^= value * 0x6c50b47c;
        value ^= value * 0xb82f1e52;
        value ^= value * 0xc7afe638;
        value ^= value * 0x8d22f6e6;

        return value;
    }

    inline uint32_t lk_hash_v1(uint32_t value, uint32_t seed) {

        value += seed;
        value ^= value * 0x6c50b47c;
        value += value << 1;
        value ^= value * 0xb82f1e52;
        value += value << 1;
        value ^= value * 0xc7afe638;
        value += value << 1;
        value ^= value * 0x8d22f6e6;
        value += value << 1;

        return value;
    }

    inline uint32_t lk_hash_v2(uint32_t value, uint32_t seed) {

        value += seed;
        value ^= 0xfa381bd0;
        value += 0x39201bd;
        value *= 0xfb11ba93;
        value ^= value * 0x1884;
        value ^= 0x13fd1d8e;
        value ^= value * 0x8d22f6e6;
        value ^= 0xf38bd101;

        return value;
    }

    inline uint32_t lk_hash_final(uint32_t value, uint32_t seed) {

        value ^= value * 0x3d20adea;
        value += seed;
        value *= (seed >> 16) | 1;
        value ^= value * 0x05526c56;
        value ^= value * 0x53a22864;

        return value;
    }
}

inline float UintToFloat(uint32_t value) {
    return std::min(1 - 1e-10f, value * 0x1p-32f);
}

template<int scrambling_type>
inline uint32_t OwenScrambling(uint32_t value, uint32_t scramble) {

    value = ReverseBits(value);
    switch (scrambling_type) {
        case 1:
            value = lk_hash(value, scramble);
            break;
        case 2:
            value = lk_hash_v1(value, scramble);
            break;
        case 3:
            value = lk_hash_v2(value, scramble);
            break;
        case 4:
            value = lk_hash_final(value, scramble);
    }
    value = ReverseBits(value);

    return value;
}
