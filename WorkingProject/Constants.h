#pragma once

const float EPSILON = 1e-10f;

const int MAX_PATHS = 8; // 32
const int MAX_BOUNCE = 4; // 4

const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;


const uint32_t SEED = 13;

enum random_generator {
    Default, LinearCongruential, MersenneTwister, SubtractWithCarry, ShuffleOrder, Halton, Sobol
};

random_generator random_generator_type = Sobol;


const std::string random_generator_type_name = random_generator_type == Halton ? "Halton" : "Sobol";

const int CHECK_PIXEL_X = 100;
const int CHECK_PIXEL_Y = 100;


std::ofstream allDimensions("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_allDimensions.txt");
std::ofstream ePixelX("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_ePixelX_0.txt");
std::ofstream ePixelY("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_ePixelY_1.txt");
std::ofstream eLightPointX("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_eLightPointX_3.txt");
std::ofstream eLightPointY("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_eLightPointY_4.txt");
std::ofstream eGetRayX("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_eGetRayX_9.txt");
std::ofstream eGetRayY("../DimensionsTest/" + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" + std::to_string(MAX_BOUNCE) + "_eGetRayY_10.txt");
