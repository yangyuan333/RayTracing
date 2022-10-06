#pragma once

#include <cmath> // c++ version code
#include <iostream>
#include <algorithm>
#include "rtweekend.h"

using std::sqrt;

class vec3 {
public:
    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline static vec3 random(double min = 0.0, double max = 1.0) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

public:
    double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

//vec3 random_in_unit_sphere() {
//    while (true) {
//        auto p = vec3::random(-1, 1);
//        if (p.length_squared() >= 1) continue;
//        return p;
//    }
//}

vec3 toworld(vec3 norm, vec3 vec) {
    vec3 B, C;
    if (std::fabs(norm.x()) > std::fabs(norm.y())) {
        double invLen = 1.0f / std::sqrt(norm.x() * norm.x() + norm.z() * norm.z());
        //std::cout << "toworld: " << std::fabs(norm.x() * norm.x() + norm.z() * norm.z()) << std::endl;
        //std::cout << "toworld: " << std::sqrt(std::fabs(norm.x() * norm.x() + norm.z() * norm.z())) << std::endl;
        C = vec3(norm.z() * invLen, 0.0f, -norm.x() * invLen);
    }
    else {
        float invLen = 1.0f / std::sqrt(norm.y() * norm.y() + norm.z() * norm.z());
        //std::cout << "toworld: " << std::fabs(norm.y() * norm.y() + norm.z() * norm.z()) << std::endl;
        //std::cout << "toworld: " << std::sqrt(std::fabs(norm.y() * norm.y() + norm.z() * norm.z())) << std::endl;
        C = vec3(0.0f, norm.z() * invLen, -norm.y() * invLen);
    }
    B = cross(C, norm);
    return vec.x() * B + vec.y() * C + vec.z() * norm;
}

vec3 random_hemi_sphere(vec3 norm) {
    // 局部坐标系均匀采样
    double phi = 2 * pi * random_double();
    double sita = std::acos(1 - random_double()); // 弧度制
    vec3 randvec(std::sin(sita) * std::cos(phi), std::sin(sita) * std::sin(phi), std::cos(sita));
    // 转换世界坐标系
    return toworld(norm, randvec);
}

vec3 random_hemi_sphere_cosine(vec3 norm) {
    // 局部坐标系均匀采样
    double phi = 2 * pi * random_double();
    //double sita = std::acos(std::clamp(-1.0, 1.0, 1 - 2 * random_double())) / 2.0; // 弧度制
    double sita = std::acos(1 - 2 * random_double()) / 2.0; // 弧度制
    vec3 randvec(std::sin(sita) * std::cos(phi), std::sin(sita) * std::sin(phi), std::cos(sita));
    // 转换世界坐标系
    return toworld(norm, randvec);
}

vec3 random_in_unit_sphere() {
    vec3 r = random_hemi_sphere(vec3(0, 0, 1));
    return (random_double() > 0.5) ? r : -1 * r;
}

vec3 random_in_unit_disk() {
    double r = std::sqrt(random_double());
    double theta = 2 * pi * random_double();
    return vec3(r * std::cos(theta), r * std::sin(theta), 0);
}

// v就是入射方向，不用取反
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

// uv就是入射方向，不用取反
// etai_over_etat为 入射介质折射率与折射介质折射率 的比值
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}