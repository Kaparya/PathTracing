#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#define BLUE_NOISE

#include "../objects/sample_dimension.h"
#include "../objects/sampler_state.h"
#include "Sobol.h"
#include "Standard.h"
#include "Halton.h"
#include "BlueNoise.h"

#include "../precalculated_data.h"

static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex, uint32_t seed) {
    SamplerState sampler{};
    sampler.seed = seed + linearPixelIndex;
    sampler.sampleIdx = pixelSampleIndex;
    return sampler;
}

template<SampleDimension Dim>
float random_float(SamplerState &state) {
    float result;

    const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    const uint32_t base_index = dimension & 31u;
    const uint32_t base = primeNumbers[base_index];

    if (randomType == 0) {
        RANDOM_TYPE = "Standard";
        result = StandardRand();
    } else if (randomType == 1) {
        RANDOM_TYPE = "Halton";
        result = HaltonRand((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) %
                            (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base);
    } else if (randomType == 2) {
        RANDOM_TYPE = "HaltonRandomDigit";
        result = HaltonRandomDigitScrambling((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) %
                                             (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base,
                                             permutations_scrambling[base_index]);
    } else if (randomType == 3) {
        RANDOM_TYPE = "HaltonOwen";
        result = HaltonOwenScrambling((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) %
                                      (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE),
                                      base,
                                      permutations_scrambling[base_index],
                                      OwenHashes[base_index]);
    } else if (randomType == 4) {
        RANDOM_TYPE = "BlueNoise";
        result = BlueNoiseRand(state, (uint32_t) Dim);
    }
    return result;
}

#endif