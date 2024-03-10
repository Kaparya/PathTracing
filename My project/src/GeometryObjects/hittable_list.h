#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "../Textures/light.h"

#include <memory>
#include <utility>
#include <vector>

class hittable_list : public hittable {
public:
    hittable_list() = default;

    hittable_list(std::shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects_.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects_.push_back(std::move(object));
    }

    void addLight(std::shared_ptr<Light> light) {
        lights_.push_back(std::move(light));
    }

    bool hit(const ray &current_ray, interval ray_t, hit_record &record) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max();

        for (const auto &object: objects_) {
            if (object->hit(current_ray, interval(ray_t.min(), closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.time;
                record = temp_rec;
            }
        }

        return hit_anything;
    }

    const std::vector<std::shared_ptr<Light>> &Lights() const {
        return lights_;
    }

private:
    std::vector<std::shared_ptr<hittable>> objects_;
    std::vector<std::shared_ptr<Light>> lights_;
};

#endif