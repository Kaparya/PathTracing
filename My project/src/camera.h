#ifndef CAMERA_H
#define CAMERA_H

#include "UsefulThings.h"

#include "Textures/color.h"
#include "GeometryObjects/hittable.h"
#include "Textures/material.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "AdditionalLibraries/stb_image_writer.h"

class camera {
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int image_width = 100;  // Rendered image width in pixel count
    int samples_per_pixel = 50;   // Count of random samples for each pixel
    int max_bounce = 20;

    double vertical_fov = 90; // Degrees
    point3 look_from = point3(0, 0, -1);
    point3 look_at = point3(0, 0, 0);
    vec3 view_up = vec3(0, 1, 0); // Can be used to zoom in/out

    double defocus_angle = 0; // 0 - equal to turn off the depth of field
    double focus_dist = 10;   // distance bet

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
                pixels[(row * image_width + column) * 4] = static_cast<int>(
                        intensity.clamp(linear_to_gamma(pixel_color.r())) * 256);
                pixels[(row * image_width + column) * 4 + 1] = static_cast<int>(
                        intensity.clamp(linear_to_gamma(pixel_color.g())) * 256);
                pixels[(row * image_width + column) * 4 + 2] = static_cast<int>(
                        intensity.clamp(linear_to_gamma(pixel_color.b())) * 256);
                pixels[(row * image_width + column) * 4 + 3] = 255;
            }
        }

        our_clock.finish();
        rendered_image_file_ += " [" + std::to_string(our_clock.result()) + " " + our_clock.clock_measure + "].png";

        std::clog << "\rDone!                        \n" << std::flush;

#ifdef SAVE_RENDERED_IMAGE
        stbi_write_png(rendered_image_file_.c_str(), image_width, image_height_, 4, pixels.data(), 0);
#endif
    }

private:
    int image_height_;   // Rendered image height
    point3 pixel00_loc_;    // Location of pixel 0, 0
    vec3 pixel_delta_u_;  // Offset to pixel to the right
    vec3 pixel_delta_v_;  // Offset to pixel below
    std::string rendered_image_file_;
    vec3 u, v, w;

    vec3 defocus_disk_u; // horizontal
    vec3 defocus_disk_v; // vertical

    point3 center;         // Camera center

    void initialize() {

        // Initialising saving file
        int file_index = 17;
        rendered_image_file_ = "../Results/test";
        rendered_image_file_ += std::to_string(file_index) + '_';
        rendered_image_file_ += std::to_string(samples_per_pixel) + '_' + std::to_string(max_bounce);

        image_height_ = static_cast<int>(image_width / aspect_ratio);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        // Determine the camera location
        center = look_from;

        // Calculating camera basis
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vertical_fov);
        auto h = std::tan(theta / 2);

        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height_);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u_ = viewport_u / image_width;
        pixel_delta_v_ = viewport_v / image_height_;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
                center - (focus_dist * w) - (viewport_u + viewport_v) / 2.0;
        pixel00_loc_ = viewport_upper_left;

        // Camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle) / 2);
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    point3 defocus_disk_sample() const {
        auto sample = random_in_unit_disk();
        return center + (sample[0] * defocus_disk_u) + (sample[1] * defocus_disk_v);
    }

    color ray_color(const ray &current_ray, const hittable &world, int bounce) const {
        if (bounce <= 0) {
            return {0, 0, 0};
        }

        hit_record record;

        if (world.hit(current_ray, interval(0.00001, infinity), record)) {
            ray scattered;
            color attenuation;
            if (record.material->scatter(current_ray, record, attenuation, scattered)) {
                return attenuation * ray_color(scattered, world, bounce - 1);
            } else if (std::dynamic_pointer_cast<light>(record.material)) {
                return attenuation;
            }
        }

//        vec3 unit_direction = unit_vector(current_ray.direction());
//        double a = 0.5 * (unit_direction.y() + 1.0);
//        return (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1.0);
        return {0, 0, 0};
    }

    ray get_ray(int row, int column) const {
        // Get a randomly sampled camera ray for the pixel at location row, column.
        point3 point = pixel00_loc_ + (column * pixel_delta_u_) + (row * pixel_delta_v_) + pixel_sample_square();
        point3 origin = defocus_angle <= 0 ? center : defocus_disk_sample();
        double time = random_float();

        return {origin, point - origin, time};
    }

    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        return (random_float() * pixel_delta_u_) + (random_float() * pixel_delta_v_);
    }
};

#endif