#ifndef CAMERA_H
#define CAMERA_H

#include "UsefulThings.h"

#include "color.h"
#include "hittable.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "AdditionalLibraries/stb_image_writer.h"

class camera {
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int image_width = 100;  // Rendered image width in pixel count
    int samples_per_pixel = 10;   // Count of random samples for each pixel
    int max_bounce = 10;

    point3 center;         // Camera center

    void render(const hittable &world) {
        initialize();

        std::vector<uint8_t> pixels(4 * image_height_ * image_width);
        static const double normalizeCoefficient = (1.0 / samples_per_pixel);

        for (int row = 0; row < image_height_; ++row) {
            std::clog << "\rScanlines remaining: " << (image_height_ - row) << ' ' << std::flush;

            for (int column = 0; column < image_width; ++column) {

                color pixel_color(0, 0, 0);

                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    pixel_color += ray_color(get_ray(row, column), world, max_bounce);
                }

                pixel_color *= normalizeCoefficient;
                // Saving color
                static const interval intensity(0, 0.999999);
                pixels[(row * image_width + column) * 4] = static_cast<int>(intensity.clamp(pixel_color.r()) * 256);
                pixels[(row * image_width + column) * 4 + 1] = static_cast<int>(intensity.clamp(pixel_color.g()) * 256);
                pixels[(row * image_width + column) * 4 + 2] = static_cast<int>(intensity.clamp(pixel_color.b()) * 256);
                pixels[(row * image_width + column) * 4 + 3] = 255;
            }
        }
        std::clog << "\rDone!                        \n" << std::flush;
        stbi_write_png(rendered_image_file_.c_str(), image_width, image_height_, 4, pixels.data(), 0);
    }

private:
    int image_height_;   // Rendered image height
    point3 pixel00_loc_;    // Location of pixel 0, 0
    vec3 pixel_delta_u_;  // Offset to pixel to the right
    vec3 pixel_delta_v_;  // Offset to pixel below
    std::string rendered_image_file_;

    void initialize() {

        int file_index = 0;
        rendered_image_file_ = "../Results/test";
        while (FILE * file = fopen((rendered_image_file_ + std::to_string(file_index) + ".png").c_str(), "r")) {
            fclose(file);

            ++file_index;
        }
        rendered_image_file_ += std::to_string(file_index) + ".png";


        image_height_ = static_cast<int>(image_width / aspect_ratio);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height_);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u_ = viewport_u / image_width;
        pixel_delta_v_ = viewport_v / image_height_;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
                center - vec3(0, 0, focal_length) - (viewport_u + viewport_v) / 2.0;
        pixel00_loc_ = viewport_upper_left;
    }

    color ray_color(const ray &r, const hittable &world, int bounce) const {
        if (bounce <= 0) {
            return {0, 0, 0};
        }

        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            vec3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(ray(rec.point, direction), world, bounce - 1);
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    ray get_ray(int row, int column) const {
        // Get a randomly sampled camera ray for the pixel at location row, column.
        point3 point = pixel00_loc_ + (column * pixel_delta_u_) + (row * pixel_delta_v_) + pixel_sample_square();

        return {center, point - center};
    }

    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        return (random_float() * pixel_delta_u_) + (random_float() * pixel_delta_v_);
    }
};

#endif