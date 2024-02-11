#pragma once

const float EPSILON = 1e-10f;

const int MAX_PATHS = 256; // 32
const int MAX_BOUNCE = 8; // 4

const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;


const uint32_t SEED = 13;

const int CHECK_PIXEL_X = 100;
const int CHECK_PIXEL_Y = 100;


enum random_generator {
    Default, LinearCongruential, MersenneTwister, SubtractWithCarry, ShuffleOrder, Halton, Sobol
};
const random_generator random_generator_type = Sobol;

enum scrambling : int {
    RandomDigit, Owen, OwenV1, OwenV2, OwenFinal, None
};
const scrambling scrambling_type = OwenFinal;















static const std::string random_generator_type_name =
        random_generator_type == Halton ? "Halton" :
        random_generator_type == Sobol ? "Sobol" :
        "NewOne";
static const std::string scrambling_type_name =
        scrambling_type == RandomDigit ? "RandomDigits/" :
        scrambling_type == Owen ? "Owen/" :
        scrambling_type == OwenV1 ? "OwenV1/" :
        scrambling_type == OwenV2? "OwenV2/" :
        scrambling_type == OwenFinal? "OwenFinal/" :
        "";

static const std::string file_name_dim_test = "../DimensionsTest/" + scrambling_type_name
                                              + random_generator_type_name + "_" + std::to_string(MAX_PATHS) + "_" +
                                              std::to_string(MAX_BOUNCE);
std::ofstream allDimensions(file_name_dim_test + "_allDimensions.txt");
std::ofstream ePixelX(file_name_dim_test + "_ePixelX_0.txt");
std::ofstream ePixelY(file_name_dim_test + "_ePixelY_1.txt");
std::ofstream eLightPointX(file_name_dim_test + "_eLightPointX_3.txt");
std::ofstream eLightPointY(file_name_dim_test + "_eLightPointY_4.txt");
std::ofstream eGetRayX(file_name_dim_test + "_eGetRayX_9.txt");
std::ofstream eGetRayY(file_name_dim_test + "_eGetRayY_10.txt");
