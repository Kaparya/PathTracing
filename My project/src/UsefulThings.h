#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <random>
#include <memory>

#include "AdditionalLibraries/clock.h"

// Constants

//std::string current_folder = "../Assets/CornellBox/";
//std::string obj_file = "CornellBox-Original.obj";

//std::string current_folder = "../Assets/Cubes/";
//std::string obj_file = "cubes.obj";

std::string current_folder = "../Assets/Wolf/";
std::string obj_file = "wolf.obj";

#define SAVE_RENDERED_IMAGE

Clock our_clock;

const double infinity = std::numeric_limits<double>::infinity();
const double epsilon = 1e-6;
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
#include "GeometryObjects/ray.h"
#include "GeometryObjects/vec3.h"

#endif