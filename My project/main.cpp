#include "UsefulThings.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

#include "AdditionalLibraries/clock.h"

int main() {

    Clock clock;
    clock.start();

    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 640;
    cam.samples_per_pixel = 100;
    cam.max_bounce = 100;

    cam.center = point3(0, 0, 0);


    cam.render(world);

    clock.finish();
    std::cout << clock;
    return 0;
}