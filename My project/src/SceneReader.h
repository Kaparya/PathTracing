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

    std::cout << "Loading scene...";

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
        std::cout << "\rLoaded" << std::endl;

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

                bool NormalsExist = (shape.mesh.indices[index].normal_index != -1);

                auto &cur_material = materials[shape.mesh.material_ids.back()];

                vec3 position_first(attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 0],
                                    attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 1],
                                    attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 2]);

                vec3 position_second(attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 0],
                                     attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 1],
                                     attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 2]);

                vec3 position_third(attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 0],
                                    attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 1],
                                    attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 2]);

                vec3 normal_first, normal_second, normal_third;

                if (NormalsExist) {
                    normal_first = {attrib.normals[3 * shape.mesh.indices[index].normal_index + 0],
                                    attrib.normals[3 * shape.mesh.indices[index].normal_index + 1],
                                    attrib.normals[3 * shape.mesh.indices[index].normal_index + 2]};

                    normal_second = {attrib.normals[3 * shape.mesh.indices[index + 1].normal_index + 0],
                                     attrib.normals[3 * shape.mesh.indices[index + 1].normal_index + 1],
                                     attrib.normals[3 * shape.mesh.indices[index + 1].normal_index + 2]};

                    normal_third = {attrib.normals[3 * shape.mesh.indices[index + 2].normal_index + 0],
                                    attrib.normals[3 * shape.mesh.indices[index + 2].normal_index + 1],
                                    attrib.normals[3 * shape.mesh.indices[index + 2].normal_index + 2]};
                }


                color our_color = color(cur_material.ambient[0],
                                        cur_material.ambient[1],
                                        cur_material.ambient[2]);

                std::shared_ptr<Material> material;
                if (fabs(cur_material.specular[0]) + fabs(cur_material.specular[1]) +
                    fabs(cur_material.specular[2]) > epsilon) {
                    material = std::make_shared<metal>(our_color, cur_material.shininess / 900.0);
                    material->specular_exponent = cur_material.shininess / 900.0;
                } else if (fabs(cur_material.ior - 1.45) > epsilon) {
                    material = std::make_shared<dielectric>(cur_material.ior);
                } else {
                    material = std::make_shared<lambertian>(our_color);
                }
                material->ambient_color = color(cur_material.ambient[0],
                                                cur_material.ambient[1],
                                                cur_material.ambient[2]);
                material->specular_color = color(cur_material.specular[0],
                                                 cur_material.specular[1],
                                                 cur_material.specular[2]);
                material->diffuse_color = color(cur_material.diffuse[0],
                                                cur_material.diffuse[1],
                                                cur_material.diffuse[2]);
                material->emission = color(cur_material.emission[0],
                                           cur_material.emission[1],
                                           cur_material.emission[2]);

                if (NormalsExist) {
                    world.add(std::make_shared<triangle>(position_first, position_second, position_third, material,
                                                         normal_first, normal_second, normal_third));
                } else {
                    world.add(std::make_shared<triangle>(position_first, position_second, position_third, material));
                }
            }
        }

//        Cornell
        world.addLight(std::make_shared<PointLight>(point3(0, 9.5, 0), color(0.75, 0.75, 0.75), 40));

//        Scene
//        world.addLight(std::make_shared<PointLight>(point3(0, 5, 0), color(1, 1, 1), 50));

//        Wolf
//        world.addLight(std::make_shared<PointLight>(point3(3.29, 2.29, 4.88), color(1, 1, 1), 25));
//        world.addLight(std::make_shared<PointLight>(point3(3.29, -3.66, 4.88), color(1, 1, 1), 25));

        std::cout << "\rScene configured!     " << std::endl;
    }
}

#endif