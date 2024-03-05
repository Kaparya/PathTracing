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

                auto &cur_material = materials[shape.mesh.material_ids[0]];

                vec3 position_first(attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 0],
                                    attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 1],
                                    attrib.vertices[3 * shape.mesh.indices[index].vertex_index + 2]);

                vec3 position_second(attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 0],
                                     attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 1],
                                     attrib.vertices[3 * shape.mesh.indices[index + 1].vertex_index + 2]);

                vec3 position_third(attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 0],
                                    attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 1],
                                    attrib.vertices[3 * shape.mesh.indices[index + 2].vertex_index + 2]);

                color our_color = color(cur_material.ambient[0],
                                        cur_material.ambient[1],
                                        cur_material.ambient[2]);

                std::shared_ptr<Material> material;
                static auto Light = std::make_shared<light>(color(0.5, 0.5, 0.5), 3);
                if (fabs(materials[shape.mesh.material_ids[0]].ior - 1) > epsilon) {
                    material = std::make_shared<dielectric>(materials[shape.mesh.material_ids[0]].ior);
                } else if ((cur_material.emission[0] * cur_material.emission[0] +
                            cur_material.emission[1] * cur_material.emission[1] +
                            cur_material.emission[2] * cur_material.emission[2]) > epsilon) {
                    material = std::make_shared<light>(our_color,
                                                       sqrt(cur_material.emission[0] * cur_material.emission[0] +
                                                            cur_material.emission[1] * cur_material.emission[1] +
                                                            cur_material.emission[2] * cur_material.emission[2]));
                } else if (sqrt(cur_material.specular[0] * cur_material.specular[0] +
                                cur_material.specular[1] * cur_material.specular[1] +
                                cur_material.specular[2] * cur_material.specular[2]) > 1.4) {
                    material = std::make_shared<metal>(
                            color(cur_material.diffuse[0], cur_material.diffuse[1], cur_material.diffuse[2]),
                            cur_material.shininess / 1000.0);
                } else {
                    material = std::make_shared<lambertian>(color(cur_material.diffuse[0],
                                                                  cur_material.diffuse[1],
                                                                  cur_material.diffuse[2]));
                }

                world.add(std::make_shared<triangle>(position_first, position_second, position_third, material));
            }
        }

        std::cout << "\rScene configured!     " << std::endl;
    }
}

#endif