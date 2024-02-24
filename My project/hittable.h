#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
public:
    point3 point;
    vec3 normal;
    float time = 0;
    bool front_face;

    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        // outward_normal -> unit length!

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray &r, interval ray_time, hit_record &record) const = 0;
};

#endif
