#pragma once

const float EPSILON = 1e-10f;

const int MAX_PATHS = 10; // 32
const int MAX_BOUNCE = 4; // 4

const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;


const uint32_t SEED = 13;

enum random_generator {
    Default, LinearCongruential, MersenneTwister, SubtractWithCarry, ShuffleOrder, Halton, Sobol
};

random_generator random_generator_type = Halton;

const bool CHECK_PIXEL = false;
const int CHECK_PIXEL_X = 10;
const int CHECK_PIXEL_Y = 10;
