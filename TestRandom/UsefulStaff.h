#pragma once

const int MAX_BOUNCE = 8;
size_t MAX_SAMPLE = 0;

enum class Generator : uint8_t {
    Standard, Halton, BlueNoise
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
    uint32_t seed = 0;
    uint32_t sampleIdx = 0;
    uint32_t depth = 0;
};
