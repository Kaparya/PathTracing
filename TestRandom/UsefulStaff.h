#pragma once

const int MAX_BOUNCE = 8;

enum class Generator : uint8_t {
    Halton, Sobol
};

enum struct SampleDimension : uint32_t {
    ePixelX,
    ePixelY,
    eLightId,
    eLightPointX,
    eLightPointY,
    eBSDF0,
    eBSDF1,
    eBSDF2,
    eBSDF3,
    eGetRayX,
    eGetRayY,
    eRussianRoulette,
    eNUM_DIMENSIONS
};

struct SamplerState {
    uint32_t seed;
    uint32_t sampleIdx;
    uint32_t depth = 0;
};