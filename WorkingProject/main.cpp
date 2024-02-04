#include <vector>
#include <chrono>
#include <iostream>

#include "ourRandom.hpp"

#define _USE_MATH_DEFINES

#include <cmath>
#include <glm/glm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>


#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>


using namespace glm;

const float EPS = 1e-5f;

const int MAX_PATHS = 10; // 32
const int MAX_BOUNCE = 10 ; // 4

struct Ray {
    vec4 o;
    vec4 d;
};

class Camera {
public:
    Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture,
           float focus_dist) { // vfov is top to bottom in degrees
        lens_radius = aperture * 0.5f;
        float theta = vfov * float(M_PI) * (1.0f / 180.0f);
        float half_height = tan(theta * 0.5f);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    Ray get_ray(const float s, const float t, SamplerState &currentState) const {
        vec3 rd = lens_radius * random_in_unit_disk(currentState);
        vec3 offset = u * rd.x + v * rd.y;
        Ray ray;
        ray.o = vec4(origin + offset, 1.0);
        ray.d = glm::normalize(vec4(lower_left_corner + s * horizontal + t * vertical - origin - offset, 0.0));
        return ray;
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

struct Intersection {
    int shapeId = -1;
    int primitiveId = -1;
    int materialId = -1;
    vec2 uv;
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
        if (det < EPS) {
            return false;
        }
        // if (abs(det) < EPS)
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
        if (t < EPS || t > isec.t) {
            return false;
        }

        isec.shapeId = 1;
        isec.uv.x = u;
        isec.uv.y = v;
        isec.t = t;

        return true;
    }
};

int generateCameraRays(std::vector<Ray> &rays, std::vector<int> &pixCoord, const Camera &cam, int width, int height,
                       std::vector<SamplerState> &raysStates, const int path) {
    const int numRays = width * height;
    rays.reserve(numRays);
    pixCoord.reserve(numRays);

    raysStates.resize(numRays);

    // Y
    // ^
    // |
    // O---> X
    for (int h = height - 1; h >= 0; --h) {
        for (int w = 0; w < width; ++w) {
            raysStates[h * width + w] = initSampler(h * width + w, path, SEED);

            SamplerState &currentState = raysStates[h * width + w];

            float u = float(w + random<SampleDimension::ePixelX>(currentState)) / float(width);
            float v = float(h + random<SampleDimension::ePixelY>(currentState)) / float(height);

            Ray r = cam.get_ray(u, v, currentState);
            rays.push_back(r);
            pixCoord.push_back(h * width + w);
        }
    }
    return numRays;
}

static std::string GetBaseDir(const std::string &filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

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
            const int indexA = indices[i + 0];
            const int indexB = indices[i + 1];
            const int indexC = indices[i + 2];
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
    return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

vec3 hemisphereSampleCosine(const vec2 &uv) {
    float cosTheta = sqrt(clamp(1.0f - uv.x, 0.0f, 1.0f));
    float sinTheta = sqrtf(clamp(1.0f - cosTheta * cosTheta, 0.0f, 1.0f));
    float phi = uv.y * 2.0f * float(M_PI);
    // avoid coplanar to surface rays
    return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

int main() {
    std::cout << "Started\n";

    int width = 320;
    int height = 240;
//    int width = 1440;
//    int height = 1080;

    std::string modelPath = "../assets/CornellBox-Original.obj";
    std::string materialPath = "../assets/";
//    std::string modelPath = "../assets/box/cube.obj";
//    std::string materialPath = "../assets/box/";

    // std::string filename = "./media/CornellBox/CornellBox-Empty-RG.obj";
    // std::string filename = "./media/CornellBox/CornellBox-Sphere.obj";


    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::string warn;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str(), materialPath.c_str());
    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    } else {
        std::cout << "Loaded" << std::endl;

        printf("# of vertices  = %d\n", (int) (attrib.vertices.size()) / 3);
        printf("# of normals   = %d\n", (int) (attrib.normals.size()) / 3);
        printf("# of texcoords = %d\n", (int) (attrib.texcoords.size()) / 2);
        printf("# of materials = %d\n", (int) materials.size());
        printf("# of shapes    = %d\n", (int) shapes.size());
    }

    AccelStructure world;

    for (const tinyobj::shape_t &shape: shapes) {
        bool isLight = false;
        if (!strcmp(shape.name.c_str(), "light")) {
            isLight = true;
            std::cout << "found light" << std::endl;
        }

        int baseIdx = 0;
        for (int f = 0; f < shape.mesh.indices.size() / 3; ++f) {
            if (isLight) {
                world.lightsIdx.push_back(world.vertices.size());
            }
            tinyobj::index_t idx0 = shape.mesh.indices[3 * f + 0];
            tinyobj::index_t idx1 = shape.mesh.indices[3 * f + 1];
            tinyobj::index_t idx2 = shape.mesh.indices[3 * f + 2];

            int current_material_id = shape.mesh.material_ids[f];

            int f0 = idx0.vertex_index;
            int f1 = idx1.vertex_index;
            int f2 = idx2.vertex_index;

            glm::vec3 posA = {attrib.vertices[3 * f0 + 0],
                              attrib.vertices[3 * f0 + 1],
                              attrib.vertices[3 * f0 + 2]};
            glm::vec3 posB = {attrib.vertices[3 * f1 + 0],
                              attrib.vertices[3 * f1 + 1],
                              attrib.vertices[3 * f1 + 2]};
            glm::vec3 posC = {attrib.vertices[3 * f2 + 0],
                              attrib.vertices[3 * f2 + 1],
                              attrib.vertices[3 * f2 + 2]};

            world.vertices.push_back(posA);
            world.vertices.push_back(posB);
            world.vertices.push_back(posC);

            world.indices.push_back(world.indices.size());
            world.indices.push_back(world.indices.size());
            world.indices.push_back(world.indices.size());

            glm::vec3 N = normalize(glm::cross(posB - posA, posC - posA));
            world.normals.push_back(N);
            world.normals.push_back(N);
            world.normals.push_back(N);

            world.materialIds.push_back(current_material_id);
            world.materialIds.push_back(current_material_id);
            world.materialIds.push_back(current_material_id);
        }
        // for (int faceIdx = 0; faceIdx < shape.mesh.num_face_vertices.size(); ++faceIdx)
        // {
        // 	int matId = shape.mesh.material_ids[faceIdx];
        // 	int numIndices = shape.mesh.indices.size();
        // 	int numVert = shape.mesh.num_face_vertices[faceIdx];
        // 	if (isLight)
        // 	{
        // 		world.lightsIdx.push_back(world.vertices.size());
        // 	}
        // 	for (int vi = 0; vi < numVert; ++vi)
        // 	{
        // 		const auto &index = shape.mesh.indices[baseIdx + vi];
        // 		{
        // 			if (index.vertex_index != -1)
        // 			{
        // 				glm::vec3 pos = {attrib.vertices[3 * index.vertex_index + 0],
        // 								 attrib.vertices[3 * index.vertex_index + 1],
        // 								 attrib.vertices[3 * index.vertex_index + 2]};
        // 				world.vertices.push_back(pos);
        // 				world.materialIds.push_back(matId);
        // 			}
        // 			if (index.normal_index != -1)
        // 			{
        // 				glm::vec3 norm = {attrib.normals[3 * index.normal_index + 0],
        // 								  attrib.normals[3 * index.normal_index + 1],
        // 								  attrib.normals[3 * index.normal_index + 2]};
        // 				world.normals.push_back(norm);
        // 			}
        // 			if (index.texcoord_index != -1)
        // 			{
        // 				glm::vec2 uv = {attrib.texcoords[2 * index.texcoord_index + 0],
        // 								attrib.texcoords[2 * index.texcoord_index + 1]};
        // 				world.uvs.push_back(uv);
        // 			}
        // 			world.indices.push_back(world.indices.size());
        // 		}
        // 	}

        // 	int lastV = world.vertices.size();
        // 	glm::vec3 a = world.vertices[lastV - 3];
        // 	glm::vec3 b = world.vertices[lastV - 2];
        // 	glm::vec3 c = world.vertices[lastV - 1];
        // 	glm::vec3 N = normalize(glm::cross(b - a, c - a));
        // 	world.normals.push_back(N);
        // 	world.normals.push_back(N);
        // 	world.normals.push_back(N);

        // 	baseIdx += numVert;
        // }
    }

    vec3 lookfrom(0, 1.8, 7.0);
    vec3 lookat(0, 1.0, 0);
    float dist_to_focus = glm::length(lookfrom - lookat);
    float aperture = 0.0f;
    Camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(width) / float(height), aperture, dist_to_focus);

    const int numPixels = height * width;
    std::vector<uint8_t> pixels(numPixels * 4);
    std::vector<vec4> colors(numPixels, vec4(0.0f));

    for (int path = 0; path < MAX_PATHS; ++path) {

        std::clog << "\rPaths left: " << MAX_PATHS - path << "    " << std::flush;

        // generate primary rays
        std::vector<std::vector<Ray>> raysBuffers(2);
        std::vector<std::vector<int>> pixelCoordBuffers(2); // stores pixel coord as int: (h * width + w)
        std::vector<std::vector<vec3>> pathWeightBuffers(2);

        std::vector<SamplerState> raysStates;

        int numRays = generateCameraRays(raysBuffers[0], pixelCoordBuffers[0], cam, width, height, raysStates, path);
        pathWeightBuffers[0].resize(numRays);
        pathWeightBuffers[0].assign(numRays, vec3{1.0f});
        for (int bounce = 0; bounce < MAX_BOUNCE; ++bounce) {
            const int currentBufferId = bounce % 2;
            numRays = raysBuffers[currentBufferId].size();
            // intersect rays
            std::vector<Intersection> isecs(numRays);
            for (int i = 0; i < numRays; ++i) {
                bool isHit = world.chit(raysBuffers[currentBufferId][i], isecs[i]);
            }
            // shade missing
            for (int i = 0; i < numRays; ++i) {
                if (isecs[i].shapeId != 1) {
                    const int h = height - 1 - pixelCoordBuffers[currentBufferId][i] / width;
                    const int w = pixelCoordBuffers[currentBufferId][i] % width;

                    colors[h * width + w] += vec4(0.0f);
                }
            }
            // compact rays
            // eval material
            for (int i = 0; i < numRays; ++i) {
                if (isecs[i].shapeId == 1) {
                    ++raysStates[i].depth;

                    const int h = height - 1 - pixelCoordBuffers[currentBufferId][i] / width;
                    const int w = pixelCoordBuffers[currentBufferId][i] % width;

                    const auto &material = materials[isecs[i].materialId];
                    const vec3 diffuse(material.diffuse[0], material.diffuse[1], material.diffuse[2]);

                    // light itself
                    if (material.emission[0] > 0.0f) {
                        // hit light
                        if (bounce > 0) {
                            isecs[i].shapeId = 0; // kill the ray
                            continue;
                        }
                        const vec3 lightColor(material.emission[0], material.emission[1], material.emission[2]);
                        colors[h * width + w] += vec4(lightColor * pathWeightBuffers[currentBufferId][i], 1.0f);
                        continue;
                    }

                    const int indexA = world.indices[isecs[i].primitiveId];
                    const int indexB = world.indices[isecs[i].primitiveId + 1];
                    const int indexC = world.indices[isecs[i].primitiveId + 2];

                    const vec3 A = world.vertices[indexA];
                    const vec3 B = world.vertices[indexB];
                    const vec3 C = world.vertices[indexC];

                    const vec3 nA = world.normals[indexA];
                    const vec3 nB = world.normals[indexB];
                    const vec3 nC = world.normals[indexC];

                    const vec3 hitWpos =
                            (1.0f - isecs[i].uv.x - isecs[i].uv.y) * A + B * isecs[i].uv.x + C * isecs[i].uv.y;
                    const vec3 N =
                            (1.0f - isecs[i].uv.x - isecs[i].uv.y) * nA + nB * isecs[i].uv.x + nC * isecs[i].uv.y;
                    vec3 color{0.0f};
                    // sample light
                    for (int lightId = 0; lightId < world.lightsIdx.size(); ++lightId) {
                        const auto &lightMaterial = materials[world.materialIds[world.lightsIdx[lightId]]];

                        const int indexLA = world.lightsIdx[lightId] + 0;
                        const int indexLB = world.lightsIdx[lightId] + 1;
                        const int indexLC = world.lightsIdx[lightId] + 2;

                        const vec3 lA = world.vertices[indexLA];
                        const vec3 lB = world.vertices[indexLB];
                        const vec3 lC = world.vertices[indexLC];

                        const vec3 nLA = world.normals[indexLA];
                        const vec3 nLB = world.normals[indexLB];
                        const vec3 nLC = world.normals[indexLC];

                        float r0 = random<SampleDimension::eLightPointX>(raysStates[i]);
                        float r1 = random<SampleDimension::eLightPointY>(raysStates[i]);
                        vec2 luv(1.0f - sqrtf(r0), sqrtf(r0) * r1);
                        // if (luv.x + luv.y >= 1.0)
                        // {
                        // 	luv.x = 1.0f - luv.x;
                        // 	luv.y = 1.0f - luv.y;
                        // }

                        const vec3 lightWPos = (1.0f - luv.x - luv.y) * lA + lB * luv.x + lC * luv.y;
                        const vec3 lightN = normalize((1.0f - luv.x - luv.y) * nLA + nLB * luv.x + nLC * luv.y);
                        const vec3 offset{N * EPS};

                        const vec3 rayO = hitWpos + offset;
                        const vec3 rayD = normalize(lightWPos - rayO);

                        const float distToLight2 = glm::dot(lightWPos - rayO, lightWPos - rayO);
                        const float distToLight = glm::distance(lightWPos, rayO);
                        const glm::vec3 L = normalize(lightWPos - rayO);
                        const vec3 V = -L;

                        const float NdotL = glm::dot(N, L);
                        const float NLdotV = glm::dot(lightN, V);
                        float visibility = 0.0f;
                        if (NdotL > 0.0f) {
                            Ray shadow;
                            shadow.o = vec4(rayO, 1.0f);
                            shadow.d = vec4(rayD, 0.0f);
                            Intersection is;
                            bool isHit = world.chit(shadow, is);
                            visibility = (isHit && is.t < (distToLight - EPS * 1.0f)) ? 0.0f : 1.0f;
                        }
                        if (NLdotV > 0.0f) {
                            float area = glm::length(glm::cross(lC - lA, lB - lA)) * 0.5f;
                            const vec3 lightColor(lightMaterial.emission[0], lightMaterial.emission[1],
                                                  lightMaterial.emission[2]);
                            const float pdf = distToLight2 / (NLdotV * area);
                            color += visibility * NdotL * NLdotV * lightColor / (distToLight2 * pdf);
                        }
                    }
                    color *= diffuse * float(M_1_PI) * pathWeightBuffers[currentBufferId][i];
                    // color *= diffuse * pathWeightBuffers[currentBufferId][i];
                    colors[h * width + w] += vec4(color, 1.0f);
                }
            }
            const int nextBufferId = (bounce + 1) % 2;
            raysBuffers[nextBufferId].clear();
            pixelCoordBuffers[nextBufferId].clear();
            pathWeightBuffers[nextBufferId].clear();
            for (int i = 0; i < numRays; ++i) {
                if (isecs[i].shapeId == 1) {
                    ++raysStates[i].depth;

                    const int indexA = world.indices[isecs[i].primitiveId];
                    const int indexB = world.indices[isecs[i].primitiveId + 1];
                    const int indexC = world.indices[isecs[i].primitiveId + 2];

                    const vec3 A = world.vertices[indexA];
                    const vec3 B = world.vertices[indexB];
                    const vec3 C = world.vertices[indexC];

                    const vec3 nA = world.normals[indexA];
                    const vec3 nB = world.normals[indexB];
                    const vec3 nC = world.normals[indexC];

                    const vec3 hitWpos =
                            (1.0f - isecs[i].uv.x - isecs[i].uv.y) * A + B * isecs[i].uv.x + C * isecs[i].uv.y;
                    const vec3 _N = normalize(
                            (1.0f - isecs[i].uv.x - isecs[i].uv.y) * nA + nB * isecs[i].uv.x + nC * isecs[i].uv.y);

                    vec3 _T, _B;
                    BasisFromDirectionCarmack(_N, _T, _B);

                    // seed = 214013 * seed + 2531011;
                    // uint rnd = seed >> 16;

                    // uint sampleIndex = i * 16384 + (rnd & 16383);
                    // vec2 uv = hammersley2d(sampleIndex, 16384 * RAY_NUM);
                    vec2 uv{random<SampleDimension::ePixelX>(raysStates[i]),
                            random<SampleDimension::ePixelY>(raysStates[i])};

                    //vec3 rndDirection = hemisphereSampleUniform(uv);
                    vec3 rndDirection = hemisphereSampleCosine(uv);
                    vec3 rayDirection = normalize(fromLocal(rndDirection, _T, _B, _N));

                    // vec3 Dir = normalize(vec3(getRandom(), getRandom(), getRandom()));
                    // Dir = dot(N, Dir) < 0.0f ? -Dir: Dir;
                    // rayDirection = Dir;
                    const vec3 offset{_N * EPS};
                    Ray r;
                    r.o = vec4(hitWpos + offset, 1.0f);
                    r.d = vec4(rayDirection, 0.0f);
                    raysBuffers[nextBufferId].push_back(r);
                    pixelCoordBuffers[nextBufferId].push_back(pixelCoordBuffers[currentBufferId][i]);
                    const auto &material = materials[isecs[i].materialId];
                    const vec3 diffuse(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
                    //float pdfRay = 1.0f / (2.0f * float(M_PI)); // Uniform
                    float pdfRay = dot(_N, rayDirection) / (1.0f * float(M_PI)); // Uniform
                    const vec3 prevBounceWeigh = pathWeightBuffers[currentBufferId][i];
                    pathWeightBuffers[nextBufferId].push_back(
                            prevBounceWeigh * diffuse * float(M_1_PI) * dot(_N, rayDirection) / pdfRay);
                }
            }
        }
    }
    std::clog << "\rDone!             \n " << std::flush;

    const float aGamma = 2.2f;
    const float invGamma = 1.0f / aGamma;
    for (int i = 0; i < numPixels; ++i) {
        const int h = i / width;
        const int w = i % width;
        const float normalizeCoeff = (1.0f / MAX_PATHS);

        const vec3 &color = colors[i] * normalizeCoeff;

        const int r = int(std::pow(color.r, invGamma) * 255.f);
        const int g = int(std::pow(color.g, invGamma) * 255.f);
        const int b = int(std::pow(color.b, invGamma) * 255.f);

        pixels[h * (width * 4) + (w * 4) + 0] = std::min(255, std::max(0, r));
        pixels[h * (width * 4) + (w * 4) + 1] = std::min(255, std::max(0, g));
        pixels[h * (width * 4) + (w * 4) + 2] = std::min(255, std::max(0, b));
        pixels[h * (width * 4) + (w * 4) + 3] = 255;
    }
    std::string outputFileName = "../Results/Halton/";
    outputFileName += std::to_string(MAX_PATHS) + '_';
    outputFileName += std::to_string(MAX_BOUNCE) + ".png";
    stbi_write_png(outputFileName.c_str(), width, height, 4, pixels.data(), 0);

    return 0;
}