#include <iostream>

#include "UsefulThings.h"
#include "camera.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"


int main() {

    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5f, -1), 100));

    camera cam;

    cam.render(world);

    return 0;
}