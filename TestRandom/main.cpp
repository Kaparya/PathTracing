#include <iostream>
#include <iomanip>
#include <fstream>

#include "UsefulStaff.h"
#include "Halton.h"
#include "Sobol.h"

float random(SamplerState &currentState, Generator generator, uint32_t Dim) {

    const uint32_t dimension = uint32_t(Dim) + currentState.depth * uint32_t(SampleDimension::eNUM_DIMENSIONS);
    static const uint32_t primeNumbers[32] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                              67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};
    const uint32_t base = primeNumbers[dimension & 31u];

    float result = 0;
    if (generator == Generator::Halton) {
        result = HaltonRand(currentState.seed * MAX_BOUNCE + currentState.sampleIdx, base);
    } else if (generator == Generator::Sobol) {
        result = SobolRand(currentState.seed * MAX_BOUNCE + currentState.sampleIdx, 2);
    }

    return result;
}

int main() {

    size_t number_of_points;
    size_t generator_input;
    std::cout << "Input the number of points / generator type / number of dimensions:\n";
    std::cin >> number_of_points;
    std::cin >> generator_input;
    Generator generator;
    std::string string_generator;
    switch (generator_input) {
        case 0:
            generator = Generator::Halton;
            string_generator = "Halton_";
            break;
        case 1:
            generator = Generator::Sobol;
            string_generator = "Sobol_";
            break;
        default:
            break;
    }

    std::string output_name = "../" + string_generator + std::to_string(number_of_points) + ".txt";
    std::ofstream output(output_name);

    for (uint32_t dimension = 0; dimension < (uint32_t)SampleDimension::eNUM_DIMENSIONS; ++dimension) {
        output << std::setw(11) << dimension;
    }
    output << std::endl;
    for (uint32_t index = 0; index < number_of_points; ++index) {
        for (uint32_t dimension = 0; dimension < (uint32_t)SampleDimension::eNUM_DIMENSIONS; ++dimension) {
            SamplerState state = {100, index};
            float value = random(state, generator, dimension);
            output << std::fixed << std::setprecision(8) << value << ' ';
            std::to_string(dimension);
        }
        output << '\n';
    }
    return 0;
}
