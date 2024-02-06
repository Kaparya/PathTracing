#pragma once

#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "../Constants.h"
#include "Halton.h"
#include "Sobol.h"

using namespace glm;

static std::ofstream fout("../DimensionsTest/output.txt");
static std::ofstream fout0("../DimensionsTest/0.txt");
static std::ofstream fout1("../DimensionsTest/1.txt");
static std::ofstream fout3("../DimensionsTest/3.txt");
static std::ofstream fout4("../DimensionsTest/4.txt");
static std::ofstream fout9("../DimensionsTest/9.txt");
static std::ofstream fout10("../DimensionsTest/10.txt");


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

    auto dim1 = uint32_t(Dim);
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

    if (MAX_PATHS <= 100 && currentState.seed == SEED + 100 * IMAGE_WIDTH + 100) {
        switch (uint32_t(Dim)) {
            case 0:
                fout0 << result << '\n';
                break;
            case 1:
                fout1 << result << '\n';
                break;
            case 3:
                fout3 << result << '\n';
                break;
            case 4:
                fout4 << result << '\n';
                break;
            case 9:
                fout9 << result << '\n';
                break;
            case 10:
                fout10 << result << '\n';
                break;
        }
        fout << result << ' ' << uint32_t(Dim) << ' ' << currentState.sampleIdx << ' ' << currentState.depth << '\n';
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
//    do {
//        p = 2.0f * vec3(random<SampleDimension::eGetRayX>(currentState),
//                        random<SampleDimension::eGetRayY>(currentState), 0) - vec3(1, 1, 0);
//    } while (dot(p, p) >= 1.0);
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
