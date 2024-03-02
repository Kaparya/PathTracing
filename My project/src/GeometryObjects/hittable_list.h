#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "../aabb.h"

#include <memory>
#include <utility>
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;

    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
        bounding_box_ = aabb(bounding_box_, object->bounding_box());
    }

    bool hit(const ray &current_ray, interval ray_t, hit_record &record) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max();

        for (const auto &object: objects) {
            if (object->hit(current_ray, interval(ray_t.min(), closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.time;
                record = temp_rec;
            }
        }

        return hit_anything;
    }

    aabb bounding_box() const override { return bounding_box_; }

private:
    aabb bounding_box_;
};

#endif