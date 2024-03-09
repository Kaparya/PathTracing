#include "UsefulThings.h"

#include "camera.h"
#include "GeometryObjects/hittable_list.h"
#include "SceneReader.h"

int main() {

    our_clock.start();

    current_folder = "../Assets/CornellBox/";
    obj_file = "CornellBox-Original.obj";

//    current_folder = "../Assets/CornellBoxMod/";
//    obj_file = "CornellBox-Original.obj";
//
//    current_folder = "../Assets/Cubes/";
//    obj_file = "cubes.obj";

//    current_folder = "../Assets/TestNormals/";
//    obj_file = "sphere.obj";

//    current_folder = "../Assets/Wolf/";
//    obj_file = "wolf.obj";

    hittable_list world;
    ReadScene(world);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 640;
    cam.samples_per_pixel = 50;
    cam.max_bounce = 10;
    MAX_BOUNCE = cam.max_bounce;
    cam.seed = 0;

    {
        std::ifstream camera_data(current_folder + "data.txt");

        camera_data >> cam.vertical_fov;
        double x, y, z;
        camera_data >> x >> y >> z;
        cam.look_from = point3(x, y, z);
        camera_data >> x >> y >> z;
        cam.look_at = point3(x, y, z);
        cam.view_up = vec3(0, 1, 0);

        camera_data.close();
    }

    cam.defocus_angle = 0;
    cam.focus_dist = 10.0;

    cam.render(world);
    return 0;
}