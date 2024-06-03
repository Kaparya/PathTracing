#include "UsefulThings.h"

#include "camera.h"
#include "GeometryObjects/hittable_list.h"
#include "SceneReader.h"

#include "Random/BlueNoise.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

int main() {
    system("clear");
    std::cout << "===========================\n";
    std::cout << "======= 3D renderer =======\n";
    std::cout << "===========================" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("clear");

    int sceneIndex = 0;
    std::cout << "====== Choose scene ======\n";
    std::cout << "0. Cornell Box\n";
    std::cout << "1. Sphere\n";
    std::cout << "2. Wolf\n";
    std::cout << "3. Wolf smoothed\n";
    std::cout << "Input the number from 0 to 3: ";
    std::cout << std::flush;
    while (std::cin >> sceneIndex) {
        if (sceneIndex >= 0 && sceneIndex <= 4) {
            break;
        }
        std::cout << "Wrong input!\nInput the number from 0 to 3: " << std::flush;
    }
    system("clear");

    current_folder = PROJECT_PATH;
    switch (sceneIndex) {
        case 0:
            current_folder += "Assets/CornellBox/";
            obj_file = "CornellBox-Original.obj";
            SCENE_NAME = "cornell_box_original_";
            break;
        case 1:
            current_folder += "Assets/TestNormals/";
            obj_file = "sphere.obj";
            SCENE_NAME = "sphere_";
            break;
        case 2:
            current_folder += "Assets/Wolf/";
            obj_file = "wolf.obj";
            SCENE_NAME = "wolf_";
            break;
        default:
            current_folder += "Assets/Wolf/";
            obj_file = "wolf_smooth.obj";
            SCENE_NAME = "wolf_smooth_";
    }

    std::cout << "====== Basic settings ======\n";
    std::cout << "\nInput the number of samples per pixel: ";
    std::cin >> MAX_SAMPLES;
    std::cout << "\nInput max number of bounces of the ray: ";
    std::cin >> MAX_BOUNCE;
    system("clear");

    our_clock.start();

    BlueNoiseGenerate(MAX_SAMPLES + MAX_BOUNCE);

    hittable_list world;
    ReadScene(world);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = IMAGE_WIDTH;
    cam.samples_per_pixel = MAX_SAMPLES;
    cam.max_bounce = MAX_BOUNCE;
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