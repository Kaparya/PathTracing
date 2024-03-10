#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#include "Sobol.h"

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
    eRayTime,
    eNUM_DIMENSIONS
};

struct SamplerState {
    uint32_t seed = 0;
    uint32_t sampleIdx = 0;
    uint32_t depth = 0;
};

static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex, uint32_t seed) {
    SamplerState sampler{};
    sampler.seed = seed + linearPixelIndex;
    sampler.sampleIdx = pixelSampleIndex;
    return sampler;
}

template<SampleDimension Dim>
float random_float(SamplerState &state) {
    float result = 0;

#ifdef SOBOL

    const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                              67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};
    const uint32_t base = primeNumbers[dimension & 31u];

    result = SobolRand(state.seed * MAX_BOUNCE + state.sampleIdx, base);

#endif
#ifdef HALTON

    const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                              67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};
    const uint32_t base = primeNumbers[dimension & 31u];

    result = HaltonRand(state.seed * MAX_BOUNCE + currentState.sampleIdx, base);

#endif
#ifdef STANDART

    static std::uniform_real_distribution<float> distribution(0, 1);
    if (uint32_t(Dim) == 0) {
        static std::mt19937 generator(0);
        result = distribution(generator);
    } else if (uint32_t(Dim) == 1) {
        static std::mt19937 generator(10);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 2) {
        static std::mt19937 generator(20);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 3) {
        static std::mt19937 generator(30);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 4) {
        static std::mt19937 generator(40);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 5) {
        static std::mt19937 generator(50);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 6) {
        static std::mt19937 generator(60);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 7) {
        static std::mt19937 generator(70);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 8) {
        static std::mt19937 generator(80);
        result =  distribution(generator);
    } else if (uint32_t(Dim) == 9) {
        static std::mt19937 generator(90);
        result =  distribution(generator);
    }

#endif

    return result;
}

#endif