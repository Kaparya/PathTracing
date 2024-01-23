#pragma once

#include <iostream>
#include "glm/glm.hpp"

using namespace glm;

extern const int MAX_PATHS;
extern const int MAX_BOUNCE;
const int SEED = 137;

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
};

inline static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex, uint32_t seed) {
    SamplerState sampler{};
    sampler.seed = seed;
    sampler.sampleIdx = pixelSampleIndex;
    return sampler;
}

inline float HaltonRand(uint32_t value, const uint32_t base) {
    float cur_pow = 1;
    float result = 0;

    while (value > 0) {
        cur_pow = cur_pow / float(base);
        result = result + cur_pow * float(value % base);
        value /= base;
    }

    return result;
}

inline float SobolRandom(uint32_t &prev_x, uint32_t &cur_index) {

    ++cur_index;

    uint32_t c_now = 1;
    unsigned value = cur_index;
    while (value & 1) {
        value >>= 1;
        ++c_now;
    }

    prev_x = prev_x ^ (1 << (32 - c_now));
    return (float) prev_x / pow(2, 32);
}

template<SampleDimension Dim>
inline float random(SamplerState &state) {

    if (random_generator_type == Halton) {

        static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                                  67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};

        const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
        const uint32_t base = primeNumbers[dimension & 31u];
        ++state.depth;
        return HaltonRand(state.seed + state.sampleIdx + state.depth * MAX_PATHS, base);
    } else if (random_generator_type == Sobol) {
        return SobolRandom(state.seed, state.depth);
    }

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
