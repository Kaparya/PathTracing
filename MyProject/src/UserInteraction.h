#ifndef USER_INTERACTION_H
#define USER_INTERACTION_H

#include "UsefulThings.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

void getSettingsFromUser() {
    system("clear");
    std::cout << "===========================\n";
    std::cout << "======= 3D renderer =======\n";
    std::cout << "===========================" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("clear");

    std::cout << "====== Choose scene ======\n";
    std::cout << "0. Cornell Box\n";
    std::cout << "1. Sphere\n";
    std::cout << "2. Wolf\n";
    std::cout << "3. Wolf smoothed\n";
    std::cout << "Input the number from 0 to 3: ";
    std::cout << std::flush;
    while (std::cin >> sceneIndex) {
        if (sceneIndex >= 0 && sceneIndex <= 3) {
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

    std::cout << "====== Choose random generator type ======\n";
    std::cout << "0. Standard Sampler\n";
    std::cout << "1. Halton Sampler\n";
    std::cout << "2. Halton with Random Digit scrambling\n";
    std::cout << "3. Halton with Owen scrambling\n";
    std::cout << "4. Blue Noise Sampler\n";
    std::cout << "Input the number from 0 to 4: ";
    std::cout << std::flush;
    while (std::cin >> randomType) {
        if (randomType >= 0 && randomType <= 4) {
            break;
        }
        std::cout << "Wrong input!\nInput the number from 0 to 4: " << std::flush;
    }
    system("clear");
}

#endif