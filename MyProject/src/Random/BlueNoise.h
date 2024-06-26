#ifndef BLUENOISE_H
#define BLUENOISE_H

#include "../precalculated_data.h"
#include "../UsefulThings.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

struct Point2F {
    int x = 0, y = 0;
};
struct Node {
    unsigned long long energy = ULLONG_MAX;
    Point2F point;
};
std::vector<std::vector<unsigned long long>> energy_mask(BLUE_NOISE_TEXTURE_SIZE,
                                                         std::vector<unsigned long long>(BLUE_NOISE_TEXTURE_SIZE, 0));

void PaintAround(Point2F point) {

    energy_mask[point.x][point.y] = ULLONG_MAX;
    const int delta = BLUE_NOISE_TEXTURE_SIZE / 2 - 8;
    for (int delta_x = -delta; delta_x <= delta; ++delta_x) {
        int current_x = (point.x + delta_x + BLUE_NOISE_TEXTURE_SIZE) % BLUE_NOISE_TEXTURE_SIZE;
        for (int delta_y = -delta; delta_y <= delta; ++delta_y) {
            if (delta_x == 0 && delta_y == 0) {
                continue;
            }
            int current_y = (point.y + delta_y + BLUE_NOISE_TEXTURE_SIZE) % BLUE_NOISE_TEXTURE_SIZE;
            double energy_value = delta - std::sqrt(abs(delta_x) * abs(delta_x) + abs(delta_y) * abs(delta_y));
            if (energy_value <= 0) {
                continue;
            }
            energy_value = energy_value / delta * 128;
            energy_value = energy_value * energy_value;
            if (energy_mask[current_x][current_y] != ULLONG_MAX) {
                energy_mask[current_x][current_y] += energy_value;
            }
        }
    }

}

// Jax Void and cluster algorithm
void BlueNoiseGenerate(uint32_t seed) {

    uint32_t not_used_number = BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE;

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distribution(0, BLUE_NOISE_TEXTURE_SIZE - 1);

    double point_index = 0;
    for (uint32_t index = 0; index < 16; ++index) {
        Point2F point = {distribution(rng), distribution(rng)};
        blue_noise_texture[point.x][point.y] = point_index / (BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE);
        point_index++;
        PaintAround(point);
        --not_used_number;
    }

    while (not_used_number) {

        Node current;
        for (int i = 0; i < BLUE_NOISE_TEXTURE_SIZE; ++i) {
            for (int j = 0; j < BLUE_NOISE_TEXTURE_SIZE; ++j) {
                if (current.energy > energy_mask[i][j]) {
                    current.energy = energy_mask[i][j];
                    current.point = {i, j};
                }
            }
        }

        blue_noise_texture[current.point.x][current.point.y] =
                point_index / (BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE);
        point_index++;

        PaintAround(current.point);
        --not_used_number;
    }

    std::ofstream raw_output("../GraphBuilder/data.txt");
    raw_output << std::fixed << std::setprecision(6);
    std::ofstream raw_output_mask("../GraphBuilder/mask.txt");
    for (uint32_t index = 0; index < BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE; ++index) {
        if (index % BLUE_NOISE_TEXTURE_SIZE == 0 && index != 0) {
            raw_output << '\n';
            raw_output_mask << '\n';
        }
        raw_output << blue_noise_texture[index / BLUE_NOISE_TEXTURE_SIZE][index % BLUE_NOISE_TEXTURE_SIZE] << ' ';
        raw_output_mask << std::setw(15)
                        << energy_mask[index / BLUE_NOISE_TEXTURE_SIZE][index % BLUE_NOISE_TEXTURE_SIZE]
                        << ' ';
    }
    raw_output_mask.close();
    raw_output.close();
}

float BlueNoiseRand(const SamplerState &currentState, uint32_t Dim) {
    size_t y = currentState.seed / IMAGE_WIDTH;
    size_t x = currentState.seed % IMAGE_WIDTH;
    size_t row = (y + (uint32_t) Dim * 100937 + currentState.sampleIdx * 1091 + currentState.depth * 133337) %
                 blue_noise_texture.size();
    size_t column = (x + (uint32_t) Dim * 99523 + currentState.sampleIdx * 2399 + currentState.depth * 133337) %
                    blue_noise_texture[0].size();
    return blue_noise_texture[row][column];
}

#endif