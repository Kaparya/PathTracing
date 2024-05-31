#ifndef HALTON_H
#define HALTON_H

const float EPSILON = 1e-6;

static float HaltonRand(uint32_t value, const uint32_t base) {
    const float invBase = (float) 1 / (float) base;
    float result = 0;
    float invBaseN = invBase;

    while (value) {
        uint32_t next = value / base;
        uint32_t digit = value - next * base;
        result += invBaseN * digit;
        invBaseN *= invBase;
        value = next;
    }

    return result;
}

static float HaltonRandomDigitScrambling(uint32_t value, const uint32_t base,
                                         const std::vector<uint32_t> &permutation) {
    const float invBase = (float) 1 / (float) base;
    float result = 0;
    float invBaseN = invBase;

    while (value) {
        uint32_t next = value / base;
        uint32_t digit = (value - next * base);

        digit = permutation[digit];

        result += invBaseN * digit;
        invBaseN *= invBase;
        value = next;
    }

    return std::max(std::min(result, 1.f - EPSILON), 0.f);
}

namespace {
    uint32_t MixBits(uint32_t key) {
        key ^= (key >> 17);
        key *= 0xed558ccd;
        key ^= (key >> 17);
        key *= 0x1a85ec53;
        key ^= (key >> 17);

        return key;
    }
};

static float HaltonOwenScrambling(uint32_t value, const uint32_t base,
                                  const std::vector<uint32_t> &permutation, uint32_t hash) {
    const float invBase = (float) 1 / (float) base;
    uint32_t result = 0;
    float invBaseN = 1;

    while (value) {
        uint32_t next = value / base;
        uint32_t digit = value - next * base;

        uint32_t digit_hash = MixBits(hash ^ result);
        digit = permutation[(digit + digit_hash) % permutation.size()];

        result = result * base + digit;
        invBaseN *= invBase;
        value = next;
    }

    return std::max(std::min(result * invBaseN, 1.f - EPSILON), 0.f);
}

#endif
