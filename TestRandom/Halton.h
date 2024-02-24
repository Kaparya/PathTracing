#pragma once

extern const uint32_t DIMENSIONS_NUMBER = 32;

static float HaltonRand(uint32_t index, const uint32_t base) {
    const float invBase = (float) 1 / (float) base;
    float value = 0;
    float invBaseN = invBase;

    while (index) {
        value += invBaseN * (index % base);
        invBaseN *= invBase;
        index /= base;
    }

    return value;
}

void InitDigitPermutation(int base, uint32_t seed) {

}

std::vector<uint32_t> nDigits(DIMENSIONS_NUMBER);
