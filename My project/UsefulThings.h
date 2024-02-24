#ifndef USEFUL_THINGS_H
#define USEFUL_THINGS_H

#include <cmath>
#include <random>
#include <limits>
#include <memory>

// Constants

const int IMAGE_WIDTH = 640;
const float ASPECT_RATIO = 16.f / 9.f;
const int SAMPLES_PER_PIXEL = 1;
const int MAX_BOUNCE = 100;



const int IMAGE_HEIGHT = static_cast<int>((IMAGE_WIDTH / ASPECT_RATIO) < 1 ? 1 : (IMAGE_WIDTH / ASPECT_RATIO));

const float INF = std::numeric_limits<float>::infinity();
const float INF_SMALL = 1e-10;
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.f;
}

inline float random_float() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0 - INF_SMALL);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    return min + random_float() * (max - min);
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
