#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <random>
#include <memory>

#include "AdditionalLibraries/clock.h"

// Constants

int MAX_BOUNCE;
std::string current_folder = "../Assets/CornellBox/";
std::string obj_file = "CornellBox-Original.obj";

#define SAVE_RENDERED_IMAGE

Clock our_clock;

const double infinity = std::numeric_limits<double>::infinity();
const double epsilon = 1e-6;
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "interval.h"
#include "GeometryObjects/ray.h"
#include "GeometryObjects/vec3.h"
#include "Random/random_numbers.h"

#endif