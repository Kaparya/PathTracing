#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <random>
#include <memory>

#include "AdditionalLibraries/clock.h"

// Constants

std::string RANDOM_TYPE;
std::string SCENE_NAME;

int sceneIndex = 0;
int randomType = 0;

int MAX_SAMPLES = 8;
int MAX_BOUNCE = 8;

const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 360;

// Correct path for CLion build
const std::string PROJECT_PATH = "../";

// If you ran with .exe file, then change this path
// to the path from your user, to PathTracing/MyProject directory
// const std::string PROJECT_PATH = "PathTracing/MyProject/";

std::string current_folder = PROJECT_PATH + "Assets/CornellBox/";
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
