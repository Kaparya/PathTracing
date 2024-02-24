#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() = default;

    ray(const point3 &origin, const vec3 direction) : origin_(origin), direction_(direction) {}

    point3 origin() const { return origin_; }

    vec3 direction() const { return direction_; }

    point3 at(float time) const {
        return origin_ + direction_ * time;
    }

private:
    point3 origin_;
    vec3 direction_;
};

#endif
