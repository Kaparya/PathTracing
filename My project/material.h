#ifndef MATERIAL_H
#define MATERIAL_H

#include "UsefulThings.h"

class hit_record;

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const ray &in_ray, const hit_record &record, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public Material {
public:
    lambertian(color albedo) : albedo_(albedo) {}

    bool scatter(const ray &in_ray, const hit_record &record, color &attenuation, ray &scattered) const override {
        vec3 scatter = record.normal + random_unit_vector();

        if (scatter.near_zero()) {
            scatter = record.normal;
        }

        scattered = ray(record.point, scatter);
        attenuation = albedo_;
        return true;
    }

private:
    color albedo_;
};

class metal : public Material {
public:
    metal(const color &albedo, const double &fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &in_ray, const hit_record &record, color &attenuation, ray &scattered) const override {
        vec3 reflected = reflect(in_ray.direction(), record.normal);
        scattered = ray(record.point, reflected + fuzz_ * random_unit_vector());
        attenuation = albedo_;
        return dot(scattered.direction(), record.normal) > 0;
    }

private:
    color albedo_;
    double fuzz_;
};

#endif