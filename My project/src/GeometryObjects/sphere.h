#ifndef SPHERE_H
#define SPHERE_H

#include <utility>
#include <iomanip>

#include "hittable.h"
#include "vec3.h"
#include "../interval.h"

class sphere : public hittable {
public:
    sphere(point3 center, double radius, std::shared_ptr<Material> current_material) :
            center_(center), radius_(radius), material_(std::move(current_material)) {}

    // If the radius is negative, then all normals  points inward, as a result the sphere is hollow

    bool hit(const ray &current_ray, interval time, hit_record &record) const override {
        vec3 oc = current_ray.origin() - center_;
        double a = current_ray.direction().length_squared();
        double half_b = dot(oc, current_ray.direction());
        double c = oc.length_squared() - radius_ * radius_;

        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) {
            return false;
        }
        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root_time = (-half_b - sqrtd) / a;
        if (!time.surrounds(root_time)) {
            root_time = (-half_b + sqrtd) / a;
            if (!time.surrounds(root_time)) {
                return false;
            }
        }

        record.time = root_time;
        record.point = current_ray.at(record.time);
        vec3 outward_normal = (record.point - center_) / radius_;
        record.set_face_normal(current_ray, outward_normal);
        record.material = material_;

        return true;
    }

private:
    point3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
};

#endif