#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
public:
    vec3() : data_{0, 0, 0} {}

    vec3(float x, float y, float z) : data_{x, y, z} {}

    float x() const { return data_[0]; }

    float y() const { return data_[1]; }

    float z() const { return data_[2]; }

    vec3 operator-() const { return {-data_[0], -data_[1], -data_[2]}; }

    float operator[](int i) const { return data_[i]; }

    float &operator[](int i) { return data_[i]; }

    vec3 &operator+=(const vec3 &rhs) {
        data_[0] += rhs[0];
        data_[1] += rhs[1];
        data_[2] += rhs[2];
        return *this;
    }

    vec3 &operator*=(float value) {
        data_[0] *= value;
        data_[1] *= value;
        data_[2] *= value;
        return *this;
    }

    vec3 &operator/=(float value) {
        return *this *= 1 / value;
    }

    float length() const {
        return std::sqrt(length_squared());
    }

    float length_squared() const {
        return data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2];
    }

    static const vec3 random() {
        return {random_float(), random_float(), random_float()};
    }

    static const vec3 random(float min, float max) {
        return {random_float(min, max), random_float(min, max), random_float(min, max)};
    }

private:
    float data_[3];
};

using point3 = vec3;

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

inline vec3 operator*(float t, const vec3 &v) {
    return {t * v[0], t * v[1], t * v[2]};
}

inline vec3 operator*(const vec3 &v, float t) {
    return t * v;
}

inline vec3 operator/(vec3 v, float t) {
    return (1 / t) * v;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u[0] * v[0]
           + u[1] * v[1]
           + u[2] * v[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return {u[1] * v[2] - u[2] * v[1],
            u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0]};
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() < 1 && p.length_squared() > INF_SMALL) {
            return unit_vector(p);
        }
    }
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
    auto vector_on_sphere = random_unit_vector();
    return dot(normal, vector_on_sphere) > 0 ? vector_on_sphere : -vector_on_sphere;
}

#endif
