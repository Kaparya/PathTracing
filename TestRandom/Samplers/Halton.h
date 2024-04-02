#pragma once

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

static float HaltonRandScrambled(uint32_t value, const uint32_t base, const std::vector<uint32_t> &permutation) {
    const float invBase = (float) 1 / (float) base;
    float result = 0;
    float invBaseN = invBase;

    size_t index = 0;
    while (value) {
        uint32_t next = value / base;
        uint32_t digit = (value - next * base);

        digit = permutation[digit];

        result += invBaseN * digit;
        invBaseN *= invBase;
        value = next;
    }

    return std::max(std::min(result, 1.f - EPS), 0.f);
}

