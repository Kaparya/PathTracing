#pragma once

#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "../Constants.h"
#include "Halton.h"
#include "Sobol.h"

using namespace glm;

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

inline static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex) {
    SamplerState sampler{};
    sampler.seed = SEED + linearPixelIndex;
    sampler.sampleIdx = pixelSampleIndex;
    return sampler;
}

template<SampleDimension Dim>
inline float random(SamplerState &currentState) {

    const uint32_t dimension = uint32_t(Dim) + currentState.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                              67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};
    const uint32_t base = primeNumbers[dimension & 31u];

    float result = 0;
    if (random_generator_type == Halton) {
        result = HaltonRand(currentState.seed * MAX_BOUNCE + currentState.sampleIdx, base);
    } else if (random_generator_type == Sobol) {
        result = SobolRand(currentState.seed * MAX_BOUNCE + currentState.sampleIdx, base);
    }

    if (currentState.seed == SEED + CHECK_PIXEL_X * IMAGE_WIDTH + CHECK_PIXEL_Y) {
        switch (Dim) {
            case SampleDimension::ePixelX:
                ePixelX << result << '\n';
                break;
            case SampleDimension::ePixelY:
                ePixelY << result << '\n';
                break;
            case SampleDimension::eLightPointX:
                eLightPointX << result << '\n';
                break;
            case SampleDimension::eLightPointY:
                eLightPointY << result << '\n';
                break;
            case SampleDimension::eGetRayX:
                eGetRayX << result << '\n';
                break;
            case SampleDimension::eGetRayY:
                eGetRayY << result << '\n';
                break;
        }
        allDimensions << result << ' ' << uint32_t(Dim) << ' ' << currentState.sampleIdx << ' ' << currentState.depth << '\n';
    }

    if (MAX_PATHS == 10) {
        if (random_generator_type == Sobol) {
            static std::ofstream allRandom("../allRandomSobol.txt");
            allRandom << result << '\n';
        } else {
            static std::ofstream allRandom("../allRandomHalton.txt");
            allRandom << result << '\n';
        }
    }
    return result;
}

vec3 random_in_unit_disk(SamplerState &currentState) {

//    vec3 p;
//
//    do {
//        p = 2.0f * vec3(random<SampleDimension::eGetRayX>(currentState),
//                        random<SampleDimension::eGetRayY>(currentState), 0) - vec3(1, 1, 0);
//    } while (dot(p, p) >= 1.0);
//
//    return p;

    float x = random<SampleDimension::eGetRayX>(currentState);
    float y = random<SampleDimension::eGetRayY>(currentState);

    vec3 result(x, y, 0);
    while (dot(result, result) >= 1.0) {
        result[0] /= 2;
        result[1] /= 2;
    }

    return result;
}
