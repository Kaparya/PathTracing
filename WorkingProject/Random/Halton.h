
inline float HaltonRand(uint32_t value, const uint32_t base) {
    const float invBase = (float)1 / (float)base;
    uint64_t reversedDigits = 0;
    float invBaseN = 1;

    while (value) {
        uint64_t next  = value / base;
        uint64_t digit = value - next * base;
        reversedDigits = reversedDigits * base + digit;
        invBaseN *= invBase;
        value = next;
    }

    return std::min(reversedDigits * invBaseN, 1 - 1e-10f);
}
