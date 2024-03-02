#include "UsefulThings.h"

#include "camera.h"
#include "GeometryObjects/hittable_list.h"
#include "SceneReader.h"

int main() {

    our_clock.start();

    hittable_list world;
    ReadScene(world);

    camera cam;

//    SPHERES
//    cam.aspect_ratio = 16.0 / 9.0;
//    cam.image_width = 1200;
//    cam.samples_per_pixel = 500;
//    cam.max_bounce = 50;
//
//    cam.vertical_fov = 20;
//    cam.look_from = point3(13, 2, 3);
//    cam.look_at = point3(0, 0, 0);
//    cam.view_up = vec3(0, 1, 0);
//
//    cam.defocus_angle = 0.6;
//    cam.focus_dist = 10.0;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1920;
    cam.samples_per_pixel = 500;
    cam.max_bounce = 20;

    cam.vertical_fov = 80;
    cam.look_from = point3(0, 6, -5);
    cam.look_at = point3(0, 3, 5);
    cam.view_up = vec3(0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.render(world);
    return 0;
}