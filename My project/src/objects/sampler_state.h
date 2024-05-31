#ifndef SAMPLER_STATE_H
#define SAMPLER_STATE_H

#include "cstddef"

struct SamplerState {
    uint32_t seed = 0;
    uint32_t sampleIdx = 0;
    uint32_t depth = 0;
};

#endif