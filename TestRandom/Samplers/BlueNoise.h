#ifndef BLUENOISE_H
#define BLUENOISE_H

struct Point2F {
    float x = 0, y = 0;
    float color = 0;
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

// Mitchell's Best Candidate Algorithm
// https://blog.demofox.org/2017/10/20/generating-blue-noise-sample-points-with-mitchells-best-candidate-algorithm/
std::vector<Point2F> BlueNoise(size_t samples_number, uint32_t seed, uint32_t base) {
    std::vector<Point2F> result_points;

    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> distribution(0, 1);

    result_points.push_back({distribution(rng), distribution(rng), 0}); // first point

    for (size_t sample = 1; sample < samples_number; ++sample) {

        Point2F best_point;
        float max_distance = -1;

        for (size_t current_test = 0; current_test < base * result_points.size(); ++current_test) {

            Point2F test_point({distribution(rng), distribution(rng), (float)sample / 256});
            float distance = 3;

            for (const auto &point: result_points) {
                distance = std::min(distance, PointDistance(point, test_point));
            }

            if (distance > max_distance) {
                max_distance = distance;
                best_point = test_point;
            }
        }

        result_points.push_back(best_point);

    }

    return result_points;
}

#endif