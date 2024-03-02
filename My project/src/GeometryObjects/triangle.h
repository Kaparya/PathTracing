#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../UsefulThings.h"

#include "hittable.h"
#include "../Textures/material.h"

class triangle : public hittable {
public:
    triangle(point3 a, point3 b, point3 c, std::shared_ptr<Material> material) : vertexes_{a, b, c},
                                                                                 material_(std::move(material)) {
        normal_ = unit_vector(cross(c - a, b - a));
        plane_coefficient_D_ = -dot(normal_, a);

        bounding_box_ = aabb(a, b);
        bounding_box_ = aabb(bounding_box_, aabb(a, c));
    }

    bool hit(const ray &current_ray, interval time, hit_record &record) const override {

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
        point3 a, b, c;
        a = vertexes_[0];
        b = vertexes_[1];
        c = vertexes_[2];

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
        record.set_face_normal(current_ray, normal_);
        record.point = point + record.normal * epsilon;
        record.material = material_;
        return true;
    }

    aabb bounding_box() const override { return bounding_box_; }

private:
    std::array<point3, 3> vertexes_;
    vec3 normal_;
    double plane_coefficient_D_;
    std::shared_ptr<Material> material_;

    aabb bounding_box_;
};

#endif