#ifndef OUR_RANDOM_H
#define OUR_RANDOM_H

#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "../Constants.h"
#include "Standard.h"
#include "Sobol.h"
#include "Halton.h"

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
    uint32_t seed = 0;
    uint32_t sampleIdx = 0;
    uint32_t depth = 0;
};

inline static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex) {
    SamplerState sampler;
    sampler.seed = SEED + linearPixelIndex;
    sampler.sampleIdx = pixelSampleIndex;
    return sampler;
}

static const std::vector<uint32_t> RandomDigitScrambling = {432184344, 3123020165, 1712321849, 712353037, 2387471059,
                                                            2384934338,
                                                            823984981, 455117590, 631653891, 473658509, 810543653,
                                                            3849875751,
                                                            1859591936};

template<SampleDimension Dim>
float random(SamplerState &currentState) {

    const uint32_t dimension = uint32_t(Dim) + currentState.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    const uint32_t base_index = dimension & 31u;
    const uint32_t base = primeNumbers[base_index];

    float result = 0;
    switch (random_generator_type) {
        case Standard: {
            result = StandardRand();
            break;
        }
        case Halton: {
            result = HaltonRand(currentState.seed * MAX_BOUNCE + currentState.sampleIdx, base);
            break;
        }
        case HaltonRandomDigit: {
            result = HaltonRandScrambled(currentState.seed * MAX_BOUNCE + currentState.sampleIdx, base, permutations_scrambling[base_index]);
            break;
        }
        case Sobol: {
            result = SobolRand(currentState.seed * MAX_PATHS + currentState.sampleIdx, base);
        }
    }

    if (DebugOutput) {
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
            allDimensions << result << ' ' << uint32_t(Dim) << ' ' << currentState.sampleIdx << ' '
                          << currentState.depth
                          << '\n';
        }

        static size_t index = 0;
        if (MAX_PATHS == 8 && index++ < 10000) {
            switch (random_generator_type) {
                case Standard: {
                    static std::ofstream allRandom("../allRandomStandard.txt");
                    allRandom << result << '\n';
                    break;
                }
                case Halton: {
                    static std::ofstream allRandom("../allRandomHalton.txt");
                    allRandom << result << '\n';
                    break;
                }
                case HaltonRandomDigit: {
                    static std::ofstream allRandom("../allRandomHaltonRandomDigit.txt");
                    allRandom << result << '\n';
                    break;
                }
                case Sobol: {
                    static std::ofstream allRandom("../allRandomSobol.txt");
                    allRandom << result << '\n';
                    break;
                }
            }
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

#endif
