cmake_minimum_required(VERSION 3.27)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project main.cpp
        AdditionalFiles/stb_image_writer.h

        SceneObjects/camera.h
        SceneObjects/ray.h
        SceneObjects/sphere.h
        SceneObjects/vec3.h

        ObjectCharacteristics/color.h
        ObjectCharacteristics/hittable_list.h
        ObjectCharacteristics/hittable.h
        ObjectCharacteristics/material.h

)
