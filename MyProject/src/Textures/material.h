#ifndef MATERIAL_H
#define MATERIAL_H

#include "../UsefulThings.h"
#include "../GeometryObjects/hittable.h"

class Material {
public:
    vec3 albedo;

    virtual ~Material() = default;

    virtual bool scatter(const ray &in_ray, const hit_record &record, ray &scattered,
                         SamplerState &state) const = 0;

    color ambient_color;     // Ka
    color emission;          // Ke
    color diffuse_color;     // Kd
    color specular_color;    // Ks
    double specular_exponent = 10; // Ns
    double Tr = 0;          // transparency
    double d = 1 - Tr;      // dissolve
    double Ni = 1;              // refraction index
};

class lambertian : public Material {
public:
    lambertian(const color &albedo_) {
        albedo = albedo_;
    }

    bool scatter(const ray &in_ray, const hit_record &record, ray &scattered,
                 SamplerState &state) const override {
        vec3 scatter = record.normal + random_unit_vector(state);

        if (scatter.near_zero()) {
            scatter = record.normal;
        }

        scattered = ray(record.point, scatter, in_ray.time());
        return true;
    }
};

class metal : public Material {
public:
    metal(const color &albedo_, double fuzz) : fuzz_(fuzz < 1 ? fuzz : 1) {
        albedo = albedo_;
    }

    bool scatter(const ray &in_ray, const hit_record &record, ray &scattered,
                 SamplerState &state) const override {
        vec3 reflected = reflect(in_ray.direction(), record.normal);
        scattered = ray(record.point, reflected + fuzz_ * random_unit_vector(state), in_ray.time());
        return dot(scattered.direction(), record.normal) > 0;
    }

private:
    double fuzz_;
};

class dielectric : public Material {
public:
    dielectric(double index_of_refraction) : index_of_refraction_(index_of_refraction) {}

    bool scatter(const ray &in_ray, const hit_record &record, ray &scattered,
                 SamplerState &state) const override {
        double refraction_ratio = record.front_face ? (1.0 / index_of_refraction_) : index_of_refraction_;

        auto unit_in_direction = unit_vector(in_ray.direction());
        double cos_theta = std::fmin(dot(-unit_in_direction, record.normal), 1.0);
        double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

        vec3 direction;
        if (refraction_ratio * sin_theta > 1.0 ||
            reflectance(cos_theta, refraction_ratio) > random_float<SampleDimension::eBSDF0>(state)) {
            direction = reflect(unit_in_direction, record.normal);
        } else {
            direction = refract(unit_in_direction, record.normal, refraction_ratio);
        }

        scattered = ray(record.point, direction, in_ray.time());
        return true;
    }

private:
    double index_of_refraction_;

    static double reflectance(double cos, double refraction_ratio) {
        auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cos), 5);
    }
};

#endif