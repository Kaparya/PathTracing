#ifndef SCENE_READER_H
#define SCENE_READER_H

#include "GeometryObjects/hittable_list.h"
#include "UsefulThings.h"
#include "Textures/color.h"
#include "GeometryObjects/sphere.h"
#include "GeometryObjects/triangle.h"
#include "Textures/material.h"

void ReadScene(hittable_list &world) {
    using std::make_shared;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));

    auto glass = make_shared<dielectric>(1.5);
    auto glass_rev = make_shared<dielectric>(1 / 1.5);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.1);

    auto red = make_shared<lambertian>(color(0.86, 0.078, 0.0117));
    auto green = make_shared<lambertian>(color(0.1328, 0.543, 0.1328));
    auto blue = make_shared<lambertian>(color(0.254, 0.41, 0.8789));

    auto white_light = make_shared<light>(color(1, 1, 1), 7);
    auto purple_light = make_shared<light>(color(0.6, 0.2627, 0.898), 3);

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
    world.add(make_shared<sphere>(point3(1, 3, 1), point3(3, 3, 1), 0.5, material2));
    world.add(make_shared<sphere>(point3(-1, 4, 7.5), 0.5, purple_light));

    // UpLight
    world.add(make_shared<triangle>(point3(2, 8, 0), point3(-2, 8, 0), point3(0, 9.5, 2), white_light));

    // Right mirror
    world.add(make_shared<triangle>(point3(-6, 2, 0), point3(-6, 7, 0), point3(-6, 2, 5), material3));

    // BOX
    // front
    world.add(make_shared<triangle>(point3(-1, 0, 0), point3(-3, 0, 0), point3(-1, 2, 0), glass));
    world.add(make_shared<triangle>(point3(-3, 2, 0), point3(-3, 0, 0), point3(-1, 2, 0), glass_rev));
    // left
    world.add(make_shared<triangle>(point3(-1, 0, 0), point3(-1, 0, 2), point3(-1, 2, 0), glass));
    world.add(make_shared<triangle>(point3(-1, 2, 0), point3(-1, 0, 2), point3(-1, 2, 2), glass));
    // up
    world.add(make_shared<triangle>(point3(-1, 2, 0), point3(-1, 2, 2), point3(-3, 2, 0), glass));
    world.add(make_shared<triangle>(point3(-3, 2, 2), point3(-1, 2, 2), point3(-3, 2, 0), glass_rev));
    // back
    world.add(make_shared<triangle>(point3(-1, 0, 2), point3(-3, 0, 2), point3(-1, 2, 2), glass));
    world.add(make_shared<triangle>(point3(-3, 2, 2), point3(-3, 0, 2), point3(-1, 2, 2), glass_rev));
    // right
    world.add(make_shared<triangle>(point3(-3, 0, 0), point3(-3, 0, 2), point3(-3, 2, 0), glass));
    world.add(make_shared<triangle>(point3(-3, 2, 0), point3(-3, 0, 2), point3(-3, 2, 2), glass));
}

#endif