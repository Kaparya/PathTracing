#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "UsefulThings.h"

#include "GeometryObjects/hittable.h"
#include "GeometryObjects/hittable_list.h"

class bvh_node : public hittable {
public:
    bvh_node(const hittable_list &list) : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(const std::vector<std::shared_ptr<hittable>> &src_objects, size_t start, size_t end) {
        auto objects = src_objects;

        int axis = random_float(0, 2.999);
        auto comparator = (axis == 0) ? box_x_compare :
                          (axis == 1) ? box_y_compare :
                          box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left_ = right_ = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                left_ = objects[start];
                right_ = objects[start + 1];
            } else {
                left_ = objects[start + 1];
                right_ = objects[start];
            }
        } else {
            std::sort(objects.begin(), objects.end(), comparator);

            auto mid = start + object_span / 2;
            left_ = std::make_shared<bvh_node>(objects, start, mid);
            right_ = std::make_shared<bvh_node>(objects, mid, end);
        }

        bouncing_box_ = aabb(left_->bounding_box(), right_->bounding_box());
    }

    bool hit(const ray &current_ray, interval time, hit_record &record) const override {
        if (!bouncing_box_.hit(current_ray, time)) {
            return false;
        }

        bool hit_left = left_->hit(current_ray, time, record);
        bool hit_right = right_->hit(current_ray, interval(time.min(), hit_left ? record.time : time.max()), record);

        return hit_left || hit_right;
    }

    static bool box_compare(const std::shared_ptr<hittable> lhs, const std::shared_ptr<hittable> rhs, int axis_index) {
        return lhs->bounding_box().axis(axis_index).min() < rhs->bounding_box().axis(axis_index).min();
    }

    static bool box_x_compare(const std::shared_ptr<hittable> lhs, const std::shared_ptr<hittable> rhs) {
        return box_compare(lhs, rhs, 0);
    }

    static bool box_y_compare(const std::shared_ptr<hittable> lhs, const std::shared_ptr<hittable> rhs) {
        return box_compare(lhs, rhs, 1);
    }

    static bool box_z_compare(const std::shared_ptr<hittable> lhs, const std::shared_ptr<hittable> rhs) {
        return box_compare(lhs, rhs, 2);
    }

    aabb bounding_box() const override { return bouncing_box_; }

private:
    std::shared_ptr<hittable> left_;
    std::shared_ptr<hittable> right_;
    aabb bouncing_box_;
};

#endif