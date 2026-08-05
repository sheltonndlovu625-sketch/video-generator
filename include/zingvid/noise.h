#pragma once
#include "math.h"
#include <cstdint>

namespace zv {

class NoiseGenerator {
public:
    explicit NoiseGenerator(uint32_t seed = 0);

    float valueNoise(float x, float y) const;
    float valueNoise(float x, float y, float z) const;
    float gradientNoise(float x, float y) const;
    float gradientNoise(float x, float y, float z) const;
    float fbm(float x, float y, int octaves = 4, float lacunarity = 2.0f, float gain = 0.5f) const;
    float fbm3D(float x, float y, float z, int octaves = 4) const;
    float voronoi(float x, float y, float* out_dx = nullptr, float* out_dy = nullptr) const;
    float simplex(float x, float y) const;
    float simplex(float x, float y, float z) const;
    float ridged(float x, float y, int octaves = 4) const;

private:
    uint32_t seed_;
    float hash(float x, float y) const;
    float hash(float x, float y, float z) const;
    Vec2 grad2(float x, float y) const;
    Vec3 grad3(float x, float y, float z) const;
};

} // namespace zv
