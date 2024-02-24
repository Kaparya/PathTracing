#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere(point3 center, float radius) : center_(center), radius_(radius) {}

    bool hit(const ray &r, interval ray_time, hit_record &record) const override {
        auto OC = r.origin() - center_;
        auto a = r.direction().length_squared();
        auto half_b = dot(r.direction(), OC);
        auto c = OC.length_squared() - radius_ * radius_;

        float discriminant = half_b * half_b - a * c;

        if (discriminant < 0) {
            return false;
        }

        float time = (-half_b - std::sqrt(discriminant)) / a; // nearest point
        if (!ray_time.surrounds(time)) {
            time = (-half_b + std::sqrt(discriminant)) / a;
            if (!ray_time.surrounds(time)) {
                return false;
            }
        }

        record.time = time;
        record.point = r.at(time);
        vec3 outward_normal = (record.point - center_) / radius_;
        record.set_face_normal(r, outward_normal);
        return true;
    }

private:
    point3 center_;
    float radius_;
};

#endif
