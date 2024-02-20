
float UintToFloat(uint32_t value) {
    return std::min(1 - 1e-10f, value * 0x1p-32f);
}

static float SobolRand(uint32_t index, uint32_t dimension) {

    uint32_t value = 0;

    return UintToFloat(value);
}