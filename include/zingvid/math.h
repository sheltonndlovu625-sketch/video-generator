#pragma once
#include <cmath>
#include <array>
#include <algorithm>

namespace zv {

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };

struct Mat4 {
    std::array<float, 16> m{};
    static Mat4 identity();
    static Mat4 perspective(float fov, float aspect, float near, float far);
    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    static Mat4 translate(const Vec3& t);
    static Mat4 rotate(float angle, const Vec3& axis);
    static Mat4 scale(const Vec3& s);
    Mat4 operator*(const Mat4& o) const;
    Vec3 transform(const Vec3& v) const;
};

struct AABB {
    Vec3 min, max;
    bool contains(const Vec3& p) const;
    AABB merge(const AABB& o) const;
};

inline float clamp(float v, float lo, float hi) {
    return std::max(lo, std::min(hi, v));
}

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return {lerp(a.x,b.x,t), lerp(a.y,b.y,t), lerp(a.z,b.z,t)};
}

inline float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

} // namespace zv
