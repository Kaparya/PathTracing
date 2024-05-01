#ifndef BLUENOISE_H
#define BLUENOISE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../../My project/src/AdditionalLibraries/stb_image_writer.h"

#include <algorithm>
#include <fstream>
#include <iomanip>

struct Point2F {
    int x = 0, y = 0;
};
struct Node {
    float energy = 1e7;
    Point2F point;
};

std::vector<std::vector<float>> blue_noise_texture(BLUE_NOISE_TEXTURE_SIZE,
                                                   std::vector<float>(BLUE_NOISE_TEXTURE_SIZE, 0));
std::vector<std::vector<float>> energy_mask(BLUE_NOISE_TEXTURE_SIZE, std::vector<float>(BLUE_NOISE_TEXTURE_SIZE, 0));

void PaintAround(Point2F point) {

    energy_mask[point.x][point.y] = 1e5;
    const int delta = BLUE_NOISE_TEXTURE_SIZE / 2;
    for (int delta_x = -delta; delta_x <= delta; ++delta_x) {
        int current_x = (point.x + delta_x + BLUE_NOISE_TEXTURE_SIZE) % BLUE_NOISE_TEXTURE_SIZE;
        for (int delta_y = -delta; delta_y <= delta; ++delta_y) {
            if (delta_x == 0 && delta_y == 0) {
                continue;
            }
            int current_y = (point.y + delta_y + BLUE_NOISE_TEXTURE_SIZE) % BLUE_NOISE_TEXTURE_SIZE;
            energy_mask[current_x][current_y] += 500 / std::pow(sqrt(delta_x * delta_x +
                                                                     delta_y * delta_y + 1), 2);
        }
    }

}

// Jax Void and cluster algorithm
void BlueNoiseGenerate(uint32_t seed) {

    std::vector<std::vector<bool>> used(BLUE_NOISE_TEXTURE_SIZE, std::vector<bool>(BLUE_NOISE_TEXTURE_SIZE, false));
    uint32_t not_used_number = BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE;

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distribution(0, BLUE_NOISE_TEXTURE_SIZE - 1);

    for (uint32_t index = 0; index < BLUE_NOISE_TEXTURE_SIZE / 8; ++index) {
        Point2F point = {distribution(rng), distribution(rng)};
        PaintAround(point);
        --not_used_number;
    }

    float point_index = 0;
    while (not_used_number) {

        Node current;
        for (int i = 0; i < BLUE_NOISE_TEXTURE_SIZE; ++i) {
            for (int j = 0; j < BLUE_NOISE_TEXTURE_SIZE; ++j) {
                if (!used[i][j] && current.energy > energy_mask[i][j]) {
                    current.energy = energy_mask[i][j];
                    current.point = {i, j};
                }
            }
        }

        used[current.point.x][current.point.y] = true;
        blue_noise_texture[current.point.x][current.point.y] =
                point_index++ / (BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE);

        PaintAround(current.point);
        --not_used_number;
    }


    std::ofstream raw_output("../GraphBuilder/data.txt");
    raw_output << std::fixed << std::setprecision(6);
    for (uint32_t index = 0; index < BLUE_NOISE_TEXTURE_SIZE * BLUE_NOISE_TEXTURE_SIZE; ++index) {
        if (index % BLUE_NOISE_TEXTURE_SIZE == 0 && index != 0) {
            raw_output << '\n';
        }
        raw_output << blue_noise_texture[index / BLUE_NOISE_TEXTURE_SIZE][index % BLUE_NOISE_TEXTURE_SIZE] << ' ';
    }
    raw_output.close();
}

#endif