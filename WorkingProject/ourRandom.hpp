#pragma once

#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "Halton.h"
#include "Sobol.h"

using namespace glm;

extern const int MAX_PATHS;
extern const int MAX_BOUNCE;
const uint32_t SEED = 1033;

enum random_generator {
    Default, LinearCongruential, MersenneTwister, SubtractWithCarry, ShuffleOrder, Halton, Sobol
};
random_generator random_generator_type = Halton;

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
    eRussianRoulette,
    eNUM_DIMENSIONS
};

struct SamplerState {
    uint32_t seed;
    uint32_t sampleIdx;
    uint32_t depth = 0;
    uint32_t now = 0;
};

inline static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex, uint32_t seed) {
    SamplerState sampler{};
    sampler.seed = seed;
    sampler.sampleIdx = pixelSampleIndex;
    return sampler;
}

template<SampleDimension Dim>
inline float random(SamplerState &state) {

    const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                              67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};
    static const uint32_t primeSums[32] = {2, 5, 10, 17, 28, 41, 58, 77, 100, 129, 160, 197, 238, 281, 328, 381,
                                           440, 501, 568, 639, 712, 791, 874, 963, 1060, 1161, 1264, 1371, 1480,
                                           1593, 1720, 1851};
    const uint32_t base = primeNumbers[dimension & 31u];

    float result = 0;
    if (random_generator_type == Halton) {
        result = HaltonRand((state.seed + state.sampleIdx) * MAX_BOUNCE + state.now, base);
    } else if (random_generator_type == Sobol) {
        result = SobolRand((state.seed + state.sampleIdx) * MAX_BOUNCE + state.now, base);
    }

    static std::ofstream fout("../output.txt");
    fout << result << '\n';
    ++state.now;
    return result;

//    static std::linear_congruential_engine<uint32_t, 48271, uint32_t(Dim), 2147483647> generator(
//            state.seed + state.sampleIdx + state.depth * MAX_PATHS);
//    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
//    ++state.depth;

    return 1;
}

vec3 random_in_unit_disk(SamplerState &currentState) {

    vec3 p;
    do {
        p = 2.0f * vec3(random<SampleDimension::eRussianRoulette>(currentState),
                        random<SampleDimension::eRussianRoulette>(currentState), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}
