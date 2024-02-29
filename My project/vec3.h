#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
public:
    vec3() : e_{0, 0, 0} {}

    vec3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

    double x() const { return e_[0]; }

    double y() const { return e_[1]; }

    double z() const { return e_[2]; }

    double r() const { return e_[0]; }

    double g() const { return e_[1]; }

    double b() const { return e_[2]; }

    vec3 operator-() const { return {-e_[0], -e_[1], -e_[2]}; }

    double operator[](int i) const { return e_[i]; }

    double &operator[](int i) { return e_[i]; }

    vec3 &operator+=(const vec3 &v) {
        e_[0] += v[0];
        e_[1] += v[1];
        e_[2] += v[2];
        return *this;
    }

    vec3 &operator*=(double t) {
        e_[0] *= t;
        e_[1] *= t;
        e_[2] *= t;
        return *this;
    }

    vec3 &operator/=(double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
    }

    bool near_zero() const {
        return (fabs(e_[0]) < epsilon) && (fabs(e_[1]) < epsilon) && (fabs(e_[2]) < epsilon);
    }

    static vec3 random() {
        return {random_float(), random_float(), random_float()};
    }

    static vec3 random(double min, double max) {
        return {random_float(min, max), random_float(min, max), random_float(min, max)};
    }

private:
    double e_[3];
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return {u[0] + v[0], u[1] + v[1], u[2] + v[2]};
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return {u[0] - v[0], u[1] - v[1], u[2] - v[2]};
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return {u[0] * v[0], u[1] * v[1], u[2] * v[2]};
}

inline vec3 operator*(double t, const vec3 &v) {
    return {t * v[0], t * v[1], t * v[2]};
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return {u[1] * v[2] - u[2] * v[1],
            u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0]};
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() < 1) {
            return p;
        }
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0) {// In the same hemisphere as the normal
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

vec3 reflect(const vec3 &in_ray, const vec3 &normal) {
    return in_ray - 2 * dot(in_ray, normal) * normal;
}

vec3 refract(const vec3& in_ray, const vec3& normal, double eta) {
    double cos_theta = std::fmin(dot(-in_ray, normal), 1.0);
    vec3 perpendicular = eta * (in_ray + cos_theta * normal);
    vec3 parallel = -sqrt(std::fabs(1 - perpendicular.length_squared())) * normal;
    return perpendicular + parallel;
}

#endif