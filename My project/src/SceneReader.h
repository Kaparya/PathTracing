#ifndef SCENE_READER_H
#define SCENE_READER_H

#include <vector>

#include "GeometryObjects/hittable_list.h"
#include "UsefulThings.h"
#include "Textures/color.h"
#include "GeometryObjects/sphere.h"
#include "GeometryObjects/triangle.h"
#include "Textures/material.h"


#define TINYOBJLOADER_IMPLEMENTATION

#include "AdditionalLibraries/tiny_obj_loader.h"


bool ReadScene(hittable_list &world) {

    std::cout << "Loading scene...\n";

    std::string modelPath = current_folder + obj_file;
    std::string materialPath = current_folder;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::string warn;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str(), materialPath.c_str());
    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    } else {
        std::cout << "Loaded" << std::endl;

        printf("# of vertices   = %d\n", (int) (attrib.vertices.size()) / 3);
        printf("# of normals    = %d\n", (int) (attrib.normals.size()) / 3);
        printf("# of tex coords = %d\n", (int) (attrib.texcoords.size()) / 2);
        printf("# of materials  = %d\n", (int) materials.size());
        printf("# of shapes     = %d\n", (int) shapes.size());

        std::cout << "Scene configuration...";

        // face
        for (const auto &shape: shapes) {

            // three numbers pack (v/vt/vn)
            for (size_t index = 0; index < shape.mesh.indices.size(); index += 3) {

                vec3 position_first(attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 0],
                                    attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 1],
                                    attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 2]);

                vec3 position_second(attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 0],
                                     attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 1],
                                     attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 2]);

                vec3 position_third(attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 0],
                                    attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 1],
                                    attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 2]);

                color our_color = color(materials[shape.mesh.material_ids[0]].ambient[0],
                                        materials[shape.mesh.material_ids[0]].ambient[1],
                                        materials[shape.mesh.material_ids[0]].ambient[2]);
                auto material = std::make_shared<lambertian>(our_color);

                material->ambient_color = {materials[shape.mesh.material_ids[0]].ambient[0],
                                           materials[shape.mesh.material_ids[0]].ambient[1],
                                           materials[shape.mesh.material_ids[0]].ambient[2]};
                material->emission = {materials[shape.mesh.material_ids[0]].emission[0],
                                           materials[shape.mesh.material_ids[0]].emission[1],
                                           materials[shape.mesh.material_ids[0]].emission[2]};
                material->diffuse_color = {materials[shape.mesh.material_ids[0]].diffuse[0],
                                           materials[shape.mesh.material_ids[0]].diffuse[1],
                                           materials[shape.mesh.material_ids[0]].diffuse[2]};
                material->specular_color = {materials[shape.mesh.material_ids[0]].specular[0],
                                            materials[shape.mesh.material_ids[0]].specular[1],
                                            materials[shape.mesh.material_ids[0]].specular[2]};

                static auto Light = std::make_shared<light>(color(0.5, 0.5, 0.5), 3);
                if (shape.name == "light") {
                    world.add(std::make_shared<triangle>(position_first, position_second, position_third, Light));
                } else {
                    world.add(std::make_shared<triangle>(position_first, position_second, position_third, material));
                }
            }
        }

        std::cout << "\rScene configured!     " << std::endl;
    }
}

#endif