#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "../interval.h"
#include "../aabb.h"

class Material;

class hit_record {
public:
    point3 point;
    vec3 normal;
    std::shared_ptr<Material> material;
    double time;
    bool front_face;

    void set_face_normal(const ray& current_ray, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(current_ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& current_ray, interval time, hit_record& record) const = 0;

    virtual aabb bounding_box() const = 0;
};

#endif