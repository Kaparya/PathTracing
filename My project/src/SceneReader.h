#ifndef SCENE_READER_H
#define SCENE_READER_H

#include "GeometryObjects/hittable_list.h"
#include "GeometryObjects/sphere.h"
#include "GeometryObjects/triangle.h"

#include "UsefulThings.h"
#include "bvh.h"

#include "Textures/color.h"
#include "Textures/material.h"

void ReadScene(hittable_list &world) {
    using std::make_shared;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));

    auto material1 = make_shared<dielectric>(1.5);
    auto material1_rev = make_shared<dielectric>(1 / 1.5);
    auto material2 = make_shared<lambertian>(color(0.7, 0.8, 1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.1);

    auto red = make_shared<lambertian>(color(0.86, 0.078, 0.0117));
    auto green = make_shared<lambertian>(color(0.1328, 0.543, 0.1328));
    auto blue = make_shared<lambertian>(color(0.254, 0.41, 0.8789));

    auto light_1 = make_shared<light>(color(1, 1, 1), 7);
    auto light_2 = make_shared<light>(color(0.6, 0.2627, 0.898), 3);

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
    world.add(make_shared<sphere>(point3(1, 3, 1), point3(3, 3, 1), 0.5, light_1));
    world.add(make_shared<sphere>(point3(-1, 4, 7.5), 0.5, light_2));
    world.add(make_shared<sphere>(point3(0, 2, 5), point3(0, 5, 5), 1, material2));

    // UpLight
    world.add(make_shared<triangle>(point3(2, 8, 0), point3(-2, 8, 0), point3(0, 9.5, 2), light_1));

    // Right mirror
    world.add(make_shared<triangle>(point3(-6, 2, 0), point3(-6, 7, 0), point3(-6, 2, 5), material3));

    // BOX
    // front
    world.add(make_shared<triangle>(point3(-1, 0, 0), point3(-3, 0, 0), point3(-1, 2, 0), material1));
    world.add(make_shared<triangle>(point3(-3, 2, 0), point3(-3, 0, 0), point3(-1, 2, 0), material1_rev));
    // left
    world.add(make_shared<triangle>(point3(-1, 0, 0), point3(-1, 0, 2), point3(-1, 2, 0), material1));
    world.add(make_shared<triangle>(point3(-1, 2, 0), point3(-1, 0, 2), point3(-1, 2, 2), material1));
    // up
    world.add(make_shared<triangle>(point3(-1, 2, 0), point3(-1, 2, 2), point3(-3, 2, 0), material1));
    world.add(make_shared<triangle>(point3(-3, 2, 2), point3(-1, 2, 2), point3(-3, 2, 0), material1_rev));
    // back
    world.add(make_shared<triangle>(point3(-1, 0, 2), point3(-3, 0, 2), point3(-1, 2, 2), material1));
    world.add(make_shared<triangle>(point3(-3, 2, 2), point3(-3, 0, 2), point3(-1, 2, 2), material1_rev));
    // right
    world.add(make_shared<triangle>(point3(-3, 0, 0), point3(-3, 0, 2), point3(-3, 2, 0), material1));
    world.add(make_shared<triangle>(point3(-3, 2, 0), point3(-3, 0, 2), point3(-3, 2, 2), material1));

//    SPHERES
//    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
//
//    for (int a = -11; a < 11; a++) {
//        for (int b = -11; b < 11; b++) {
//            auto choose_mat = random_float();
//            point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());
//
//            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
//                std::shared_ptr<Material> sphere_material;
//
//                if (choose_mat < 0.8) {
//                    // diffuse
//                    auto albedo = color::random() * color::random();
//                    sphere_material = make_shared<lambertian>(albedo);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                } else if (choose_mat < 0.95) {
//                    // metal
//                    auto albedo = color::random(0.5, 1);
//                    auto fuzz = random_float(0, 0.5);
//                    sphere_material = make_shared<metal>(albedo, fuzz);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                } else {
//                    // glass
//                    sphere_material = make_shared<dielectric>(1.5);
//                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
//                }
//            }
//        }
//    }
//
//    auto material1 = make_shared<dielectric>(1.5);
//    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
//
//    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
//
//    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
//
//    world = hittable_list(make_shared<bvh_node>(world));




//    std::cout << "Loading scene...\n";
//
//    std::string modelPath = "../Assets/CornellBox/CornellBox-Original.obj";
//    std::string materialPath = "../Assets/CornellBox/";
//
//
//    tinyobj::attrib_t attrib;
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//    std::string err;
//    std::string warn;
//
//    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str(), materialPath.c_str());
//    if (!warn.empty()) {
//        std::cout << warn << std::endl;
//    }
//
//    if (!err.empty()) {
//        std::cerr << err << std::endl;
//    }
//
//    if (!ret) {
//        exit(1);
//    } else {
//        std::cout << "Loaded" << std::endl;
//
//        printf("# of vertices   = %d\n", (int) (attrib.vertices.size()) / 3);
//        printf("# of normals    = %d\n", (int) (attrib.normals.size()) / 3);
//        printf("# of tex coords = %d\n", (int) (attrib.texcoords.size()) / 2);
//        printf("# of materials  = %d\n", (int) materials.size());
//        printf("# of shapes     = %d\n", (int) shapes.size());
//
//        for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
//            printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
//                   static_cast<const double>(attrib.vertices[3 * v + 0]),
//                   static_cast<const double>(attrib.vertices[3 * v + 1]),
//                   static_cast<const double>(attrib.vertices[3 * v + 2]));
//        }
//
//        for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
//            printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
//                   static_cast<const double>(attrib.normals[3 * v + 0]),
//                   static_cast<const double>(attrib.normals[3 * v + 1]),
//                   static_cast<const double>(attrib.normals[3 * v + 2]));
//        }
//
//        for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
//            printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
//                   static_cast<const double>(attrib.texcoords[2 * v + 0]),
//                   static_cast<const double>(attrib.texcoords[2 * v + 1]));
//        }
//
//        // For each shape
//        for (size_t i = 0; i < shapes.size(); i++) {
//            printf("shape[%ld].name = %s\n", static_cast<long>(i),
//                   shapes[i].name.c_str());
//            printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
//                   static_cast<unsigned long>(shapes[i].mesh.indices.size()));
//            printf("Size of shape[%ld].lines.indices: %lu\n", static_cast<long>(i),
//                   static_cast<unsigned long>(shapes[i].lines.indices.size()));
//            printf("Size of shape[%ld].points.indices: %lu\n", static_cast<long>(i),
//                   static_cast<unsigned long>(shapes[i].points.indices.size()));
//
//            size_t index_offset = 0;
//
//            assert(shapes[i].mesh.num_face_vertices.size() ==
//                   shapes[i].mesh.material_ids.size());
//
//            assert(shapes[i].mesh.num_face_vertices.size() ==
//                   shapes[i].mesh.smoothing_group_ids.size());
//
//            printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
//                   static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));
//
//            // For each face
//            for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
//                size_t fnum = shapes[i].mesh.num_face_vertices[f];
//
//                printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
//                       static_cast<unsigned long>(fnum));
//
//                // For each vertex in the face
//                for (size_t v = 0; v < fnum; v++) {
//                    tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
//                    printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
//                           static_cast<long>(v), idx.vertex_index, idx.normal_index,
//                           idx.texcoord_index);
//                }
//
//                printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
//                       shapes[i].mesh.material_ids[f]);
//                printf("  face[%ld].smoothing_group_id = %d\n", static_cast<long>(f),
//                       shapes[i].mesh.smoothing_group_ids[f]);
//
//                index_offset += fnum;
//            }
//
//            printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
//                   static_cast<unsigned long>(shapes[i].mesh.tags.size()));
//            for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
//                printf("  tag[%ld] = %s ", static_cast<long>(t),
//                       shapes[i].mesh.tags[t].name.c_str());
//                printf(" ints: [");
//                for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
//                    printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
//                    if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
//                        printf(", ");
//                    }
//                }
//                printf("]");
//
//                printf(" floats: [");
//                for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
//                    printf("%f", static_cast<const double>(
//                            shapes[i].mesh.tags[t].floatValues[j]));
//                    if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
//                        printf(", ");
//                    }
//                }
//                printf("]");
//
//                printf(" strings: [");
//                for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
//                    printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
//                    if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
//                        printf(", ");
//                    }
//                }
//                printf("]");
//                printf("\n");
//            }
//        }
//
//        for (size_t i = 0; i < materials.size(); i++) {
//            printf("material[%ld].name = %s\n", static_cast<long>(i),
//                   materials[i].name.c_str());
//            printf("  material.Ka = (%f, %f ,%f)\n",
//                   static_cast<const double>(materials[i].ambient[0]),
//                   static_cast<const double>(materials[i].ambient[1]),
//                   static_cast<const double>(materials[i].ambient[2]));
//            printf("  material.Kd = (%f, %f ,%f)\n",
//                   static_cast<const double>(materials[i].diffuse[0]),
//                   static_cast<const double>(materials[i].diffuse[1]),
//                   static_cast<const double>(materials[i].diffuse[2]));
//            printf("  material.Ks = (%f, %f ,%f)\n",
//                   static_cast<const double>(materials[i].specular[0]),
//                   static_cast<const double>(materials[i].specular[1]),
//                   static_cast<const double>(materials[i].specular[2]));
//            printf("  material.Tr = (%f, %f ,%f)\n",
//                   static_cast<const double>(materials[i].transmittance[0]),
//                   static_cast<const double>(materials[i].transmittance[1]),
//                   static_cast<const double>(materials[i].transmittance[2]));
//            printf("  material.Ke = (%f, %f ,%f)\n",
//                   static_cast<const double>(materials[i].emission[0]),
//                   static_cast<const double>(materials[i].emission[1]),
//                   static_cast<const double>(materials[i].emission[2]));
//            printf("  material.Ns = %f\n",
//                   static_cast<const double>(materials[i].shininess));
//            printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
//            printf("  material.dissolve = %f\n",
//                   static_cast<const double>(materials[i].dissolve));
//            printf("  material.illum = %d\n", materials[i].illum);
//            printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
//            printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
//            printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
//            printf("  material.map_Ns = %s\n",
//                   materials[i].specular_highlight_texname.c_str());
//            printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
//            printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
//            printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
//            printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
//            printf("  <<PBR>>\n");
//            printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
//            printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
//            printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
//            printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
//            printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_roughness));
//            printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
//            printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
//            printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
//            printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
//            printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
//            printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
//            printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
//            std::map<std::string, std::string>::const_iterator it(
//                    materials[i].unknown_parameter.begin());
//            std::map<std::string, std::string>::const_iterator itEnd(
//                    materials[i].unknown_parameter.end());
//
//            for (; it != itEnd; it++) {
//                printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
//            }
//            printf("\n");
//        }
//    }
}

#endif