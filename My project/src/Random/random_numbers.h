#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#define STANDARD

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

#ifdef SOBOL

    RANDOM_TYPE = "Sobol";

    const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                              67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};
    const uint32_t base = primeNumbers[dimension & 31u];

    result = SobolRand(state.seed * MAX_BOUNCE + state.sampleIdx, base);

#endif
#ifdef STANDARD

    RANDOM_TYPE = "Standard";

    result = StandardRand();

#endif
#ifdef HALTON

    RANDOM_TYPE = "Halton";

    result = HaltonRand((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) % (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base);

#endif
#ifdef HALTON_RANDOM_DIGIT

    RANDOM_TYPE = "HaltonRandomDigit";

    result = HaltonRandomDigitScrambling((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) % (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base,
                                         permutations_scrambling[base_index]);

#endif
#ifdef HALTON_OWEN

    RANDOM_TYPE = "HaltonOwen";

    result = HaltonOwenScrambling((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) % (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE),
                                  base,
                                  permutations_scrambling[base_index],
                                  OwenHashes[base_index]);

#endif
#ifdef BLUE_NOISE

    RANDOM_TYPE = "BlueNoise";

    result = BlueNoiseRand(state, (uint32_t)Dim);

#endif

    return result;
}

#endif