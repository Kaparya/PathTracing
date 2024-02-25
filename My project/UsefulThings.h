#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <random>
#include <memory>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0, 1);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_float();
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif