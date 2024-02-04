
const float EPS = 1e-10f;

const int MAX_PATHS = 1; // 32
const int MAX_BOUNCE = 2; // 4

const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;


const uint32_t SEED = 0;

enum random_generator {
    Default, LinearCongruential, MersenneTwister, SubtractWithCarry, ShuffleOrder, Halton, Sobol
};

random_generator random_generator_type = Halton;
