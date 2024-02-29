#include "UsefulThings.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include "AdditionalLibraries/clock.h"

int main() {

    using std::make_shared;

    Clock clock;
    clock.start();

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 640; //640
    cam.samples_per_pixel = 200;
    cam.max_bounce = 10;

    cam.vertical_fov = 60;
    cam.look_from = point3(-2, 2, 2);
    cam.look_at = point3(0, 0, -1);
    cam.view_up = vec3(0, 1, 0);

    cam.render(world);

    clock.finish();
    std::cout << clock;
    return 0;
}