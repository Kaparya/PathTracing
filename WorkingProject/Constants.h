#pragma once

const float EPSILON = 1e-10f;

const int MAX_PATHS = 100; // 32
const int MAX_BOUNCE = 8; // 4

const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;


const uint32_t SEED = 13;

enum random_generator {
    Default, LinearCongruential, MersenneTwister, SubtractWithCarry, ShuffleOrder, Halton, Sobol
};

random_generator random_generator_type = Sobol;

const int CHECK_PIXEL_X = 100;
const int CHECK_PIXEL_Y = 100;


std::ofstream allDimensions("../DimensionsTest/allDimensions.txt");
std::ofstream ePixelX("../DimensionsTest/ePixelX_0.txt");
std::ofstream ePixelY("../DimensionsTest/ePixelY_1.txt");
std::ofstream eLightPointX("../DimensionsTest/eLightPointX_3.txt");
std::ofstream eLightPointY("../DimensionsTest/eLightPointY_4.txt");
std::ofstream eGetRayX("../DimensionsTest/eGetRayX_9.txt");
std::ofstream eGetRayY("../DimensionsTest/eGetRayY_10.txt");
