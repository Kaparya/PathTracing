#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <future>

#include "UsefulThings.h"

#include "color.h"
#include "hittable.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "AdditionalLibraries/stb_image_writer.h"

class camera {
public:
    void render(const hittable &world) {
        initialize();

        std::string rendered_image_file = "../Results/test.png";
        std::vector<uint8_t> pixels(IMAGE_HEIGHT * IMAGE_WIDTH * 3);

        for (int row = 0; row < IMAGE_HEIGHT; ++row) {
            std::clog << "\rRow: " << IMAGE_HEIGHT - row << "        ";
            for (int column = 0; column < IMAGE_WIDTH; ++column) {

                auto pixel_center = pixel00_loc_ + (column * pixel_delta_u_) + (row * pixel_delta_v_);
                auto ray_direction = pixel_center - camera_center_;
                ray r(camera_center_, ray_direction);

                color pixel_color = ray_color(r, world, MAX_BOUNCE);

//                for (int sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
//                    auto current_ray = get_ray(row, column);
//                    pixel_color += ray_color(current_ray, world, MAX_BOUNCE);
//                }
//
//                static const interval borders(0.f, 1.f - INF_SMALL);
//                pixel_color[0] = borders.clamp(pixel_color[0] / SAMPLES_PER_PIXEL);
//                pixel_color[1] = borders.clamp(pixel_color[1] / SAMPLES_PER_PIXEL);
//                pixel_color[2] = borders.clamp(pixel_color[2] / SAMPLES_PER_PIXEL);

                pixels[(row * IMAGE_WIDTH + column) * 3] = uint8_t(pixel_color.x() * (256 - INF_SMALL));
                pixels[(row * IMAGE_WIDTH + column) * 3 + 1] = uint8_t(pixel_color.y() * (256 - INF_SMALL));
                pixels[(row * IMAGE_WIDTH + column) * 3 + 2] = uint8_t(pixel_color.z() * (256 - INF_SMALL));
            }
        }

        stbi_write_png(rendered_image_file.c_str(), IMAGE_WIDTH, IMAGE_HEIGHT, 3, pixels.data(), 0);
    }

private:
    point3 camera_center_;
    vec3 pixel_delta_u_;
    vec3 pixel_delta_v_;
    point3 pixel00_loc_;

    void initialize() {

        camera_center_ = point3(0, 0, 0);

        float focal_length = 1.0;
        float viewport_height = 2.0;
        auto viewport_width = viewport_height * (float(IMAGE_WIDTH) / float(IMAGE_HEIGHT));

        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // deltas
        pixel_delta_u_ = viewport_u / IMAGE_WIDTH;
        pixel_delta_v_ = viewport_v / IMAGE_HEIGHT;

        // upper-left pixel
        auto viewport_upper_left =
                camera_center_ - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }

    color ray_color(const ray &current_ray, const hittable &world, int bounce) const {
//        if (bounce <= 0) {
//            return {0, 0, 0};
//        }
//
        hit_record record;
        if (world.hit(current_ray, interval(0, INF), record)) {
//            vec3 direction = random_on_hemisphere(record.normal);
//            return 0.5 * ray_color(ray(record.point, direction), world, bounce - 1);
            return 0.5 * (record.normal + color(1, 1, 1));
        }

        vec3 unit_direction = unit_vector(current_ray.direction());
        float a = 0.5f * (unit_direction.y() + 1.0f);
        return (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1);
    }

    ray get_ray(int row, int column) const {
        auto pixel_center = pixel00_loc_ + (pixel_delta_u_ * column) + (pixel_delta_v_ * row);
        auto point = pixel_center + pixel_square_sample();
        return {camera_center_, point - camera_center_};
    }

    vec3 pixel_square_sample() const {
        float px = -0.5f + random_float();
        float py = -0.5f + random_float();
        return (pixel_delta_u_ * px) + (pixel_delta_v_ * py);
    }
};

#endif
