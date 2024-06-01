#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <bitset>

#include "UsefulThings.h"

#include "Samplers/Standard.h"
#include "Samplers/Halton.h"
#include "Samplers/BlueNoise.h"

float random(SamplerState &state, Generator generator, uint32_t Dim) {

    const uint32_t dimension = uint32_t(Dim) + state.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    const uint32_t base_index = dimension & 31u;
    const uint32_t base = primeNumbers[base_index];

    float result = 0;
    switch (generator) {
        case Generator::Standard: {
            result = StandardRand();
            break;
        }
        case Generator::Halton: {
            result = HaltonRand((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) %
                                (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base);
            break;
        }
        case Generator::HaltonRandomDigit: {
            result = HaltonRandomDigitScrambling((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) %
                                                 (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base,
                                                 permutations_scrambling[base_index]);
            break;
        }
        case Generator::HaltonOwen: {
            result = HaltonOwenScrambling((state.seed * MAX_SAMPLES * 94281923 + state.sampleIdx * 131861) %
                                          (IMAGE_HEIGHT * IMAGE_WIDTH * MAX_SAMPLES * MAX_BOUNCE), base,
                                          permutations_scrambling[base_index],
                                          OwenHashes[base_index]);
            break;
        }
        case Generator::BlueNoise: {
            result = BlueNoiseRand(state, (uint32_t)Dim);
        }
    }

    return result;
}

int main() {

    size_t number_of_points;
    size_t generator_input;
    size_t number_of_pixels;
    std::cout << "Input the number of points / generator type / number of pixels to test:\n";
    std::cin >> number_of_points;
    MAX_SAMPLES = number_of_points;
    BlueNoiseGenerate(12);
    generator_input = 4;
    std::cin >> generator_input;
    number_of_pixels = 4;
//    std::cin >> number_of_pixels;
    Generator generator;
    std::string string_generator = "../Results/";
    switch (generator_input) {
        case 0:
            generator = Generator::Standard;
            string_generator += "Standard_";
            break;
        case 1:
            generator = Generator::Halton;
            string_generator += "Halton_";
            break;
        case 2:
            generator = Generator::HaltonRandomDigit;
            string_generator += "HaltonRandomDigit_";
            break;
        case 3:
            generator = Generator::HaltonOwen;
            string_generator += "HaltonOwen_";
            break;
        case 4:
            generator = Generator::BlueNoise;
            string_generator += "BlueNoise_";
            break;
        default:
            break;
    }

    std::string output_name = string_generator + std::to_string(number_of_points) + ".txt";
    std::ofstream output;
    output.open(output_name);
    output << number_of_pixels << ' ' << (uint32_t) SampleDimension::eNUM_DIMENSIONS << '\n';

    for (auto pixel = 0; pixel < number_of_pixels; ++pixel) {
        uint32_t seed = ((IMAGE_WIDTH * IMAGE_HEIGHT) / number_of_pixels) * pixel +
                        rand() % ((IMAGE_WIDTH * IMAGE_HEIGHT) / number_of_pixels);
//        uint32_t seed = 0;
        output << seed / IMAGE_WIDTH << ' ' << seed % IMAGE_WIDTH << '\n';
        for (uint32_t dimension = 0; dimension < (uint32_t) SampleDimension::eNUM_DIMENSIONS; ++dimension) {
            output << std::setw(11) << dimension;
        }
        output << '\n';

        for (uint32_t index = 0; index < number_of_points; ++index) {
            std::cerr << "\rPoints left: " << number_of_points - index << "          ";
            for (uint32_t dimension = 0; dimension < (uint32_t) SampleDimension::eNUM_DIMENSIONS; ++dimension) {
                SamplerState state = {seed + (uint32_t) number_of_points, index};
                float value = random(state, generator, dimension);
                output << std::fixed << std::setprecision(8) << value << ' ';
            }
            output << '\n';
        }
    }
    std::cerr << "\rDone" << "                    ";

    output.close();
    return 0;
}
