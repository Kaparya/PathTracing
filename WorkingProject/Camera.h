#pragma once

#include "Geometry.h"

class Camera {
public:
    Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture,
           float focus_dist) { // vfov is top to bottom in degrees
        lens_radius = aperture * 0.5f;
        float theta = vfov * float(M_PI) * (1.0f / 180.0f);
        float half_height = tan(theta * 0.5f);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    Ray get_ray(const float s, const float t, SamplerState &currentState) const {
        vec3 rd = lens_radius * random_in_unit_disk(currentState);
        vec3 offset = u * rd.x + v * rd.y;
        Ray ray{};
        ray.o = vec4(origin + offset, 1.0);
        ray.d = glm::normalize(vec4(lower_left_corner + s * horizontal + t * vertical - origin - offset, 0.0));
        return ray;
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

inline int generateCameraRays(std::vector<Ray> &rays, std::vector<int> &pixCoord, const Camera &cam,
                       std::vector<SamplerState> &raysStates, const int path) {
    const int numRays = IMAGE_WIDTH * IMAGE_HEIGHT;
    rays.reserve(numRays);
    pixCoord.reserve(numRays);

    raysStates.resize(numRays);

    // Y
    // ^
    // |
    // O---> X
    for (int h = IMAGE_HEIGHT - 1; h >= 0; --h) {
        for (int w = 0; w < IMAGE_WIDTH; ++w) {
            raysStates[h * IMAGE_WIDTH + w] = initSampler(h * IMAGE_WIDTH + w, path);

            SamplerState &currentState = raysStates[h * IMAGE_WIDTH + w];

            float u = ((float) w + random<SampleDimension::ePixelX>(currentState)) / float(IMAGE_WIDTH);
            float v = ((float) h + random<SampleDimension::ePixelY>(currentState)) / float(IMAGE_HEIGHT);

            Ray r = cam.get_ray(u, v, currentState);
            rays.push_back(r);
            pixCoord.push_back(h * IMAGE_WIDTH + w);
        }
    }
    return numRays;
}
