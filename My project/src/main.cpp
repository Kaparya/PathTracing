#include "UsefulThings.h"

#include "camera.h"
#include "Textures/color.h"
#include "GeometryObjects/hittable_list.h"
#include "GeometryObjects/sphere.h"
#include "GeometryObjects/triangle.h"
#include "Textures/material.h"

#include "AdditionalLibraries/clock.h"

int main() {

    using std::make_shared;

    Clock clock;
    clock.start();

    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));

    auto material1 = make_shared<dielectric>(100);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.1);

    auto red = make_shared<lambertian>(color(0.86, 0.078, 0.0117));
    auto green = make_shared<lambertian>(color(0.1328, 0.543, 0.1328));
    auto blue = make_shared<lambertian>(color(0.254, 0.41, 0.8789));

    // GROUND
    world.add(make_shared<triangle>(point3(10, 0, -10), point3(10, 0, 10), point3(-10, 0, 10), ground_material));
    world.add(make_shared<triangle>(point3(10, 0, -10), point3(-10, 0, -10), point3(-10, 0, 10), ground_material));

    // MIRRORS
    world.add(make_shared<triangle>(point3(5, 0, 0), point3(0, 10, 5), point3(0, 0, 10), material3));
    world.add(make_shared<triangle>(point3(-5, 0, 0), point3(0, 10, 5), point3(0, 0, 10), material3));

    // WALLS
    world.add(make_shared<triangle>(point3(10, 0, -9), point3(10, 20, -9), point3(10, 0, 9), red));
    world.add(make_shared<triangle>(point3(10, 20, 9), point3(10, 20, -9), point3(10, 0, 9), red));

    world.add(make_shared<triangle>(point3(9, 0, 10), point3(9, 20, 10), point3(-9, 0, 10), green));
    world.add(make_shared<triangle>(point3(-9, 20, 10), point3(9, 20, 10), point3(-9, 0, 10), green));

    world.add(make_shared<triangle>(point3(-10, 0, -9), point3(-10, 20, 9), point3(-10, 0, 9), blue));
    world.add(make_shared<triangle>(point3(-10, 0, -9), point3(-10, 20, 9), point3(-10, 20, -9), blue));

    //SPHERE
    world.add(make_shared<sphere>(point3(0.5, 2, 3.5), 0.5, material3));

    // BOX
    // front
    world.add(make_shared<triangle>(point3(-1, 0, 0), point3(-3, 0, 0), point3(-1, 2, 0), blue));
    world.add(make_shared<triangle>(point3(-3, 2, 0), point3(-3, 0, 0), point3(-1, 2, 0), blue));
    // left
    world.add(make_shared<triangle>(point3(-1, 0, 0), point3(-1, 0, 2), point3(-1, 2, 0), blue));
    world.add(make_shared<triangle>(point3(-1, 2, 0), point3(-1, 0, 2), point3(-1, 2, 2), blue));
    // up
    world.add(make_shared<triangle>(point3(-1, 2, 0), point3(-1, 2, 2), point3(-3, 2, 0), blue));
    world.add(make_shared<triangle>(point3(-3, 2, 2), point3(-1, 2, 2), point3(-3, 2, 0), blue));
    // back
    world.add(make_shared<triangle>(point3(-1, 0, 2), point3(-3, 0, 2), point3(-1, 2, 2), blue));
    world.add(make_shared<triangle>(point3(-3, 2, 2), point3(-3, 0, 2), point3(-1, 2, 2), blue));
    // right
    world.add(make_shared<triangle>(point3(-3, 0, 0), point3(-3, 0, 2), point3(-3, 2, 0), blue));
    world.add(make_shared<triangle>(point3(-3, 2, 0), point3(-3, 0, 2), point3(-3, 2, 2), blue));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 640;
    cam.samples_per_pixel = 50;
    cam.max_bounce = 10;

    cam.vertical_fov = 80;
    cam.look_from = point3(0, 6, -5);
    cam.look_at = point3(0, 3, 5);
    cam.view_up = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.render(world);

//    hittable_list world;
//
//    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
//
//    for (int a = -11; a < 11; a++) {
//        for (int b = -11; b < 11; b++) {
//            auto choose_mat = random_float();
//            point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());
//
//            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//                std::shared_ptr<Material> sphere_material;
//
//                if (choose_mat < 0.8) {
//                    // diffuse
//                    auto albedo = color::random() * color::random();
//                    sphere_material = make_shared<lambertian>(albedo);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                } else if (choose_mat < 0.95) {
//                    // metal
//                    auto albedo = color::random(0.5, 1);
//                    auto fuzz = random_float(0, 0.5);
//                    sphere_material = make_shared<metal>(albedo, fuzz);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                } else {
//                    // glass
//                    sphere_material = make_shared<dielectric>(1.5);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                }
//            }
//        }
//    }
//
//    auto material1 = make_shared<dielectric>(1.5);
//    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
//
//    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
//
//    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
//
//    camera cam;
//
//    cam.aspect_ratio = 16.0 / 9.0;
//    cam.image_width = 1200;
//    cam.samples_per_pixel = 100;
//    cam.max_bounce = 10;
//
//    cam.vertical_fov = 20;
//    cam.look_from = point3(13, 2, 3);
//    cam.look_at = point3(0, 0, 0);
//    cam.view_up = vec3(0, 1, 0);
//
//    cam.defocus_angle = 0.6;
//    cam.focus_dist = 10.0;
//
//    cam.render(world);

    clock.finish();
    std::cout << clock;
    return 0;
}