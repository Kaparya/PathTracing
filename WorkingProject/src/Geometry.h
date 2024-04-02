#pragma once

struct Ray {
    vec4 o;
    vec4 d;
};

struct Intersection {
    int shapeId = -1;
    int primitiveId = -1;
    int materialId = -1;
    vec2 uv{};
    float t = 1e9f;
};

struct Triangle {
    static bool intersection(const Ray &r, const vec3 &a, const vec3 &b, const vec3 &c, Intersection &isec) {
        // O + t * D = (1 - u - v) * A + u * B + v * C
        // O + t * D = A - u * A - v * A + u * B + v * C
        // O + t * D = A + u * (B - A) + v * (C - A)
        // O - A = -t * D + u * (B - A) + v * (C - A)
        // T = O - A
        // E1 = B - A
        // E2 = C - A
        // T = -t * D + u * E1 + v * E2
        //                [t]
        // [-D, E1, E2] x [u] = T
        //                [v]
        // M = [-D, E1, E2]
        // det(M) = -D * (E1 x E2) = D * (E2 x E1) = E1 * (D x E2)
        // [t]        1          | | T, E1, E2| |
        // [u] = ------------- * | |-D, T, E2 | |
        // [v]   E1 * (D x E2)   | |-D, E1, T | |
        // regroup
        // [t]        1          | | T * (E1 x E2)| |
        // [u] = ------------- * | |-D * (T x E2) | |
        // [v]   E1 * (D x E2)   | |-D * (E1 x T) | |
        // regroup
        // [t]        1          | | E2 * (T x E1)| |
        // [u] = ------------- * | | T * (D x E2) | |
        // [v]   E1 * (D x E2)   | | D * (T x E1) | |
        // P = D x E2, Q = T x E1
        // [t]     1      | | E2 * Q| |
        // [u] = ------ * | | T * P | |
        // [v]   E1 * P   | | D * Q | |
        vec3 e1 = b - a;
        vec3 e2 = c - a;
        vec3 p = cross(vec3(r.d), e2);
        float det = dot(e1, p);
        // culling
        if (det < EPSILON) {
            return false;
        }
        // if (abs(det) < EPSILON)
        // {
        // return false;
        // }
        float invDet = 1.0f / det;
        vec3 T = vec3(r.o) - a;
        float u = dot(T, p) * invDet;
        if (u < 0.0f || u > 1.0f) {
            return false;
        }
        vec3 Q = cross(T, e1);
        float v = dot(vec3(r.d), Q) * invDet;
        if (v < 0.0f || (u + v) > 1.0f) {
            return false;
        }
        float t = dot(e2, Q) * invDet;
        // if (t < 0.0f || t > isec.t)
        if (t < EPSILON || t > isec.t) {
            return false;
        }

        isec.shapeId = 1;
        isec.uv.x = u;
        isec.uv.y = v;
        isec.t = t;

        return true;
    }
};

struct AccelStructure {
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<int> materialIds;

    std::vector<int> lightsIdx;

    bool chit(const Ray &r, Intersection &isec) {
        // traverse
        for (int i = 0; i < indices.size() - 2; i += 3) {
            const int indexA = (int) indices[i + 0];
            const int indexB = (int) indices[i + 1];
            const int indexC = (int) indices[i + 2];
            const vec3 &a = vertices[indexA];
            const vec3 &b = vertices[indexB];
            const vec3 &c = vertices[indexC];
            Intersection currentIsec;
            currentIsec.materialId = materialIds[indexA];
            currentIsec.primitiveId = i;

            bool isHit = Triangle::intersection(r, a, b, c, currentIsec);
            if (isHit && currentIsec.t < isec.t) {
                isec = currentIsec;
            }
        }
        return (isec.t < 1e9f);
    }
};

void BasisFromDirectionCarmack(const vec3 &N, vec3 &T, vec3 &B) {
    T.y = -N.x;
    //T.zx = N.yz;
    T.z = N.y;
    T.x = N.z;
    T -= dot(T, N) * N;
    T = normalize(T);
    B = cross(N, T);
}

vec3 fromLocal(const vec3 &v, const vec3 &t, const vec3 &b, const vec3 &n) {
    return t * v.x + b * v.y + n * v.z;
}

vec3 hemisphereSampleUniform(const vec2 &uv) {
    float cosTheta = (clamp(1.0f - uv.x, 0.0f, 1.0f));
    float sinTheta = sqrtf(clamp(1.0f - cosTheta * cosTheta, 0.0f, 1.0f));
    float phi = uv.y * 2.0f * float(M_PI);
    // avoid coplanar to surface rays
    return {cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta};
}

vec3 hemisphereSampleCosine(const vec2 &uv) {
    float cosTheta = sqrt(clamp(1.0f - uv.x, 0.0f, 1.0f));
    float sinTheta = sqrtf(clamp(1.0f - cosTheta * cosTheta, 0.0f, 1.0f));
    float phi = uv.y * 2.0f * float(M_PI);
    // avoid coplanar to surface rays
    return {cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta};
}
