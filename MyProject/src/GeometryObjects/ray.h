#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() = default;

    ray(const point3 &origin, const vec3 &direction) : origin_(origin),
                                                       direction_(direction),
                                                       time_(0) {}

    ray(const point3 &origin, const vec3 &direction, double time) : origin_(origin),
                                                                    direction_(direction),
                                                                    time_(time) {}

    point3 origin() const { return origin_; }

    vec3 direction() const { return direction_; }

    double time() const { return time_; }

    point3 at(double t) const {
        return origin_ + t * direction_;
    }

private:
    point3 origin_;
    vec3 direction_;
    double time_;
};

#endif