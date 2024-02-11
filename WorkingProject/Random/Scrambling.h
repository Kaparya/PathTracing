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
}

inline uint32_t OwenScrambling(uint32_t value, uint32_t scramble) {

    value = ReverseBits(value);
    value = lk_hash(value, scramble);
    value = ReverseBits(value);

    return value;
}
