#ifndef BLUENOISE_H
#define BLUENOISE_H

#include "../Constants.h"

struct Point2F {
    float x = 0, y = 0;
};

inline static float PointDistance(Point2F lhs, Point2F rhs) {

    float x_delta = lhs.x - rhs.x;
    float y_delta = lhs.y - rhs.y;

    if (x_delta > 0.5f) {
        x_delta = 1 - x_delta;
    }
    if (y_delta > 0.5f) {
        y_delta = 1 - y_delta;
    }

    return std::sqrt(x_delta * x_delta + y_delta * y_delta);
}

std::vector<Point2F> BlueNoisePoints(MAX_PATHS * (uint32_t)SampleDimension::eNUM_DIMENSIONS);

// Mitchell's Best Candidate Algorithm
// https://blog.demofox.org/2017/10/20/generating-blue-noise-sample-points-with-mitchells-best-candidate-algorithm/
void BlueNoiseGenerate() {

    size_t samples_number = BlueNoisePoints.size();
    uint32_t seed = 11;
    uint32_t base = 3;

    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> distribution(0, 1);

    BlueNoisePoints[0] = {distribution(rng), distribution(rng)}; // first point

    std::cout << "\n";
    for (size_t sample = 1; sample < samples_number; ++sample) {
        std::cerr << "\r Generating points left: " << samples_number - sample << "        ";
        Point2F best_point;
        float max_distance = -1;

        for (size_t current_test = 0; current_test < base * sample; ++current_test) {

            Point2F test_point({distribution(rng), distribution(rng)});
            float distance = 3;

            for (size_t index = 0; index < sample; ++index) {
                distance = std::min(distance, PointDistance(BlueNoisePoints[index], test_point));
            }

            if (distance > max_distance) {
                max_distance = distance;
                best_point = test_point;
            }
        }

        BlueNoisePoints[sample] = best_point;

    }
}

#endif