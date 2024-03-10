#ifndef LIGHT_H
#define LIGHT_H

#include "../GeometryObjects/vec3.h"
#include "../Textures/color.h"

class Light {
public:
    virtual ~Light() = default;

    double intensity_ = 1;
    color color_;
};

class PointLight : public Light {
public:

    PointLight(point3 origin, color our_color, double intensity) : origin_(origin) {
        intensity_ = intensity;
        color_ = our_color;
    }

    color LightIntensity(point3 point) const {
        double distance_coefficient = 1 / ((origin_ - point).length_squared() * 4 * pi);
        return color_ * intensity_ * distance_coefficient;
    }

    point3 origin_;
};

#endif