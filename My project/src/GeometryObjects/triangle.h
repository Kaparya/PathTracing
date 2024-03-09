#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <exception>

#include "../UsefulThings.h"

#include "hittable.h"
#include "../Textures/material.h"

class triangle : public hittable {
public:
    triangle(point3 a, point3 b, point3 c,
             std::shared_ptr<Material> material) : vertexes_{a, b, c}, is_moving(false),
                                                   points_normals_(false),
                                                   material_(std::move(material)) {
    }

    triangle(point3 a, point3 b, point3 c,
             point3 a_to, point3 b_to, point3 c_to,
             std::shared_ptr<Material> material) : vertexes_{a, b, c},
                                                   vertexes_shift_{a_to - a, b_to - b, c_to - c},
                                                   is_moving(true), points_normals_(false),
                                                   material_(std::move(material)) {
    }

    triangle(point3 a, point3 b, point3 c,
             std::shared_ptr<Material> material,
             vec3 normal_a, vec3 normal_b, vec3 normal_c) : vertexes_{a, b, c},
                                                            is_moving(false), points_normals_(true),
                                                            material_(std::move(material)),
                                                            normals_{normal_a, normal_b, normal_c} {
    }

    bool hit(const ray &current_ray, interval time, hit_record &record) const override {

        point3 a = is_moving ? a_at(current_ray.time()) : vertexes_[0];
        point3 b = is_moving ? b_at(current_ray.time()) : vertexes_[1];
        point3 c = is_moving ? c_at(current_ray.time()) : vertexes_[2];

        vec3 normal_ = unit_vector(cross(c - a, b - a));
        double plane_coefficient_D_ = -dot(normal_, a);

        double normal_dot_direction = dot(normal_, unit_vector(current_ray.direction()));
        if (fabs(normal_dot_direction) < epsilon) {
            return false;
        }

        double intersection_time = -(dot(normal_, current_ray.origin()) + plane_coefficient_D_) /
                                   dot(normal_, current_ray.direction());

        if (!time.surrounds(intersection_time)) {
            return false;
        }

        point3 point = current_ray.at(intersection_time);

        bool point_inside = (dot(normal_, cross(b - a, point - a)) < 0) &&
                            (dot(normal_, cross(c - b, point - b)) < 0) &&
                            (dot(normal_, cross(a - c, point - c)) < 0);

        if (point_inside) {
            point_inside = true;
        }

        if (!point_inside) {
            return false;
        }

        record.time = intersection_time;

        double shape = cross(a - b, c - b).length();
        double w0 = cross(point - b, c - b).length() / shape;
        double w1 = cross(point - c, a - c).length() / shape;

        if (shape < -epsilon || w0 < -epsilon || w1 < -epsilon) {
            throw std::logic_error("Wrong Cross product!");
        }

        if (points_normals_) {
            normal_ = unit_vector(w0 * normals_[0] +
                                  w1 * normals_[1] +
                                  (1 - w0 - w1) * normals_[2]);
        }

        record.set_face_normal(current_ray, normal_);
        record.point = point + record.normal * epsilon;
        record.material = material_;
        return true;
    }

private:
    std::array<point3, 3> vertexes_;
    std::array<point3, 3> vertexes_shift_;
    bool is_moving;
    bool points_normals_;
    std::shared_ptr<Material> material_;
    std::array<vec3, 3> normals_;

    point3 a_at(double time) const {
        return vertexes_[0] + vertexes_shift_[0] * time;
    }

    point3 b_at(double time) const {
        return vertexes_[1] + vertexes_shift_[1] * time;
    }

    point3 c_at(double time) const {
        return vertexes_[2] + vertexes_shift_[2] * time;
    }
};

#endif