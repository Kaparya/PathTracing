#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;

    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    void clear() { objects.clear(); }

    bool hit(const ray &r, interval ray_time, hit_record &record) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_time.max();

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_time.min(), closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.time;
                record = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif
