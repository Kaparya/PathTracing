#ifndef OUR_RANDOM_H
#define OUR_RANDOM_H

#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "../Constants.h"
#include "Standard.h"
#include "Halton.h"
#include "BlueNoise.h"
#include "Sobol.h"

using namespace glm;

static const std::vector<uint32_t> OwenHashes = {0xb694b1f4, 0x7eb7d41c, 0x8bf4251c, 0xf0ea7299, 0xc551ce33,
                                                 0xe75d6a6b, 0xd239f9f0, 0xcc2775fd, 0x6713db31, 0x41ecec7,
                                                 0xae553148, 0xe17f90cd, 0xe720c4d5, 0xe697db5b, 0x1296bb16,
                                                 0xe07d213c, 0x5fa8d144, 0xf3a5e3a8, 0x806b8b34, 0x3875e60d,
                                                 0xfe9d2916, 0x2da800cf, 0xa43393fb, 0xb7403e48, 0xf5547c8f,
                                                 0x324a1e77, 0x497c0deb, 0x29b4427c, 0x4b17bdff, 0xf6d78aa3,
                                                 0x746e10b4, 0x89c231e4};

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
            result = HaltonRand(currentState.seed * MAX_PATHS + currentState.sampleIdx, base);
            break;
        }
        case HaltonRandomDigit: {
            result = HaltonRandomDigitScrambling(currentState.seed * MAX_PATHS + currentState.sampleIdx, base,
                                                 permutations_scrambling[base_index]);
            break;
        }
        case HaltonOwen: {
            result = HaltonOwenScrambling(currentState.seed * MAX_PATHS + currentState.sampleIdx, base,
                                          permutations_scrambling[base_index], OwenHashes[base_index]);
            break;
        }
        case BlueNoise: {
            result = BlueNoiseRand(currentState, (uint32_t)Dim);
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
                case HaltonOwen: {
                    static std::ofstream allRandom("../allRandomHaltonOwen.txt");
                    allRandom << result << '\n';
                    break;
                }
                case Sobol: {
                    static std::ofstream allRandom("../allRandomSobol.txt");
                    allRandom << result << '\n';
                    break;
                }
                case BlueNoise: {
                    static std::ofstream allRandom("../allRandomBlueNoise.txt");
                    allRandom << result << '\n';
                    break;
                }
            }
        }
    }
    return result;
}

vec3 random_in_unit_disk(SamplerState &currentState) {

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
