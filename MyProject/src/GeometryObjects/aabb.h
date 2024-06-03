#ifndef AABB_H
#define AABB_H
/*
#include "UsefulThings.h"

class aabb {
public:

    aabb() = default;

    aabb(const interval &x, const interval &y, const interval &z) :
            x_(x), y_(y), z_(z) {}

    aabb(const point3 &first, const point3 &second) {
        x_ = interval(fmin(first[0], second[0]), fmax(first[0], second[0]));
        y_ = interval(fmin(first[1], second[1]), fmax(first[1], second[1]));
        z_ = interval(fmin(first[2], second[2]), fmax(first[2], second[2]));
    }

    aabb(const aabb& first, const aabb& second) {
        x_ = interval(first.x_, second.x_);
        y_ = interval(first.y_, second.y_);
        z_ = interval(first.z_, second.z_);
    }

    const interval &axis(int n) const {
        if (n == 1) {
            return y_;
        }
        if (n == 2) {
            return z_;
        }
        return x_;
    }

    bool hit(const ray &current_ray, interval time) const {
        for (int a = 0; a < 3; a++) {
            auto inv_direction = 1 / current_ray.direction()[a];
            auto origin = current_ray.origin()[a];

            auto t0 = (axis(a).min() - origin) * inv_direction;
            auto t1 = (axis(a).max() - origin) * inv_direction;

            if (inv_direction < 0) {
                std::swap(t0, t1);
            }

            if (t0 > time.min()) { time.min() = t0; }
            if (t1 < time.max()) { time.max() = t1; }

            if (time.max() <= time.min()) {
                return false;
            }
        }
        return true;
    }

private:
    interval x_, y_, z_;
};
*/
#endif