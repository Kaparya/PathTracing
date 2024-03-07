#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

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
float random_float(SamplerState& state) {
    static std::uniform_real_distribution<float> distribution(0, 1);
    if (uint32_t(Dim) == 0) {
        static std::mt19937 generator(0);
        return distribution(generator);
    } else if (uint32_t(Dim) == 1) {
        static std::mt19937 generator(10);
        return distribution(generator);
    } else if (uint32_t(Dim) == 5) {
        static std::mt19937 generator(50);
        return distribution(generator);
    } else if (uint32_t(Dim) == 6) {
        static std::mt19937 generator(60);
        return distribution(generator);
    } else if (uint32_t(Dim) == 9) {
        static std::mt19937 generator(90);
        return distribution(generator);
    }
}

#endif