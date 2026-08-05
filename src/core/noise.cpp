#include "zingvid/noise.h"
#include <cmath>

namespace zv {

NoiseGenerator::NoiseGenerator(uint32_t seed) : seed_(seed) {}

float NoiseGenerator::hash(float x, float y) const {
    uint32_t h = seed_;
    h ^= (uint32_t)(x * 374761393u);
    h ^= (uint32_t)(y * 668265263u);
    h = (h ^ (h >> 13)) * 1274126177u;
    h = h ^ (h >> 16);
    return (float)(h & 0x7FFFFFFF) / (float)0x7FFFFFFF;
}

float NoiseGenerator::hash(float x, float y, float z) const {
    uint32_t h = seed_;
    h ^= (uint32_t)(x * 374761393u);
    h ^= (uint32_t)(y * 668265263u);
    h ^= (uint32_t)(z * 1274126177u);
    h = (h ^ (h >> 13)) * 374761393u;
    h = h ^ (h >> 16);
    return (float)(h & 0x7FFFFFFF) / (float)0x7FFFFFFF;
}

Vec2 NoiseGenerator::grad2(float x, float y) const {
    float angle = hash(x, y) * 6.283185307f;
    return {std::cos(angle), std::sin(angle)};
}

Vec3 NoiseGenerator::grad3(float x, float y, float z) const {
    float a = hash(x, y + 127.0f) * 6.283185307f;
    float b = hash(y, z + 311.0f) * 3.141592654f;
    return {std::sin(b)*std::cos(a), std::sin(b)*std::sin(a), std::cos(b)};
}

float NoiseGenerator::valueNoise(float x, float y) const {
    float ix = std::floor(x), iy = std::floor(y);
    float fx = x - ix, fy = y - iy;
    fx = fx * fx * (3.0f - 2.0f * fx);
    fy = fy * fy * (3.0f - 2.0f * fy);

    float v00 = hash(ix, iy);
    float v10 = hash(ix+1, iy);
    float v01 = hash(ix, iy+1);
    float v11 = hash(ix+1, iy+1);

    return lerp(lerp(v00, v10, fx), lerp(v01, v11, fx), fy);
}

float NoiseGenerator::valueNoise(float x, float y, float z) const {
    float ix = std::floor(x), iy = std::floor(y), iz = std::floor(z);
    float fx = x - ix, fy = y - iy, fz = z - iz;
    fx = fx * fx * (3.0f - 2.0f * fx);
    fy = fy * fy * (3.0f - 2.0f * fy);
    fz = fz * fz * (3.0f - 2.0f * fz);

    float v000 = hash(ix, iy, iz);
    float v100 = hash(ix+1, iy, iz);
    float v010 = hash(ix, iy+1, iz);
    float v110 = hash(ix+1, iy+1, iz);
    float v001 = hash(ix, iy, iz+1);
    float v101 = hash(ix+1, iy, iz+1);
    float v011 = hash(ix, iy+1, iz+1);
    float v111 = hash(ix+1, iy+1, iz+1);

    float x1 = lerp(lerp(v000, v100, fx), lerp(v010, v110, fx), fy);
    float x2 = lerp(lerp(v001, v101, fx), lerp(v011, v111, fx), fy);
    return lerp(x1, x2, fz);
}

float NoiseGenerator::gradientNoise(float x, float y) const {
    float ix = std::floor(x), iy = std::floor(y);
    float fx = x - ix, fy = y - iy;

    Vec2 g00 = grad2(ix, iy);
    Vec2 g10 = grad2(ix+1, iy);
    Vec2 g01 = grad2(ix, iy+1);
    Vec2 g11 = grad2(ix+1, iy+1);

    float d00 = g00.x * fx + g00.y * fy;
    float d10 = g10.x * (fx-1) + g10.y * fy;
    float d01 = g01.x * fx + g01.y * (fy-1);
    float d11 = g11.x * (fx-1) + g11.y * (fy-1);

    fx = fx * fx * (3.0f - 2.0f * fx);
    fy = fy * fy * (3.0f - 2.0f * fy);

    return lerp(lerp(d00, d10, fx), lerp(d01, d11, fx), fy) * 0.5f + 0.5f;
}

float NoiseGenerator::gradientNoise(float x, float y, float z) const {
    float ix = std::floor(x), iy = std::floor(y), iz = std::floor(z);
    float fx = x - ix, fy = y - iy, fz = z - iz;

    Vec3 g000 = grad3(ix, iy, iz);
    Vec3 g100 = grad3(ix+1, iy, iz);
    Vec3 g010 = grad3(ix, iy+1, iz);
    Vec3 g110 = grad3(ix+1, iy+1, iz);
    Vec3 g001 = grad3(ix, iy, iz+1);
    Vec3 g101 = grad3(ix+1, iy, iz+1);
    Vec3 g011 = grad3(ix, iy+1, iz+1);
    Vec3 g111 = grad3(ix+1, iy+1, iz+1);

    float d000 = g000.x*fx + g000.y*fy + g000.z*fz;
    float d100 = g100.x*(fx-1) + g100.y*fy + g100.z*fz;
    float d010 = g010.x*fx + g010.y*(fy-1) + g010.z*fz;
    float d110 = g110.x*(fx-1) + g110.y*(fy-1) + g110.z*fz;
    float d001 = g001.x*fx + g001.y*fy + g001.z*(fz-1);
    float d101 = g101.x*(fx-1) + g101.y*fy + g101.z*(fz-1);
    float d011 = g011.x*fx + g011.y*(fy-1) + g011.z*(fz-1);
    float d111 = g111.x*(fx-1) + g111.y*(fy-1) + g111.z*(fz-1);

    fx = fx * fx * (3.0f - 2.0f * fx);
    fy = fy * fy * (3.0f - 2.0f * fy);
    fz = fz * fz * (3.0f - 2.0f * fz);

    float x1 = lerp(lerp(d000, d100, fx), lerp(d010, d110, fx), fy);
    float x2 = lerp(lerp(d001, d101, fx), lerp(d011, d111, fx), fy);
    return lerp(x1, x2, fz) * 0.5f + 0.5f;
}

float NoiseGenerator::fbm(float x, float y, int octaves, float lacunarity, float gain) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;
    for(int i = 0; i < octaves; i++) {
        total += gradientNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }
    return total / maxValue;
}

float NoiseGenerator::fbm3D(float x, float y, float z, int octaves) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;
    for(int i = 0; i < octaves; i++) {
        total += gradientNoise(x*frequency, y*frequency, z*frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    return total / maxValue;
}

float NoiseGenerator::voronoi(float x, float y, float* out_dx, float* out_dy) const {
    float ix = std::floor(x), iy = std::floor(y);
    float minDist = 1000.0f;
    float dx = 0, dy = 0;

    for(int j = -1; j <= 1; j++) {
        for(int i = -1; i <= 1; i++) {
            float px = ix + i + hash(ix+i, iy+j);
            float py = iy + j + hash(ix+i+127, iy+j+311);
            float d = std::sqrt((x-px)*(x-px) + (y-py)*(y-py));
            if(d < minDist) {
                minDist = d;
                dx = px - x;
                dy = py - y;
            }
        }
    }
    if(out_dx) *out_dx = dx;
    if(out_dy) *out_dy = dy;
    return minDist;
}

float NoiseGenerator::simplex(float x, float y) const {
    // 2D simplex noise
    const float F2 = 0.5f * (std::sqrt(3.0f) - 1.0f);
    const float G2 = (3.0f - std::sqrt(3.0f)) / 6.0f;

    float s = (x + y) * F2;
    float i = std::floor(x + s), j = std::floor(y + s);
    float t = (i + j) * G2;
    float X0 = i - t, Y0 = j - t;
    float x0 = x - X0, y0 = y - Y0;

    float i1, j1;
    if(x0 > y0) { i1 = 1; j1 = 0; }
    else { i1 = 0; j1 = 1; }

    float x1 = x0 - i1 + G2, y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2, y2 = y0 - 1.0f + 2.0f * G2;

    float n0 = 0, n1 = 0, n2 = 0;
    float t0 = 0.5f - x0*x0 - y0*y0;
    if(t0 >= 0) { t0 *= t0; Vec2 g = grad2(i, j); n0 = t0 * t0 * (g.x*x0 + g.y*y0); }
    float t1 = 0.5f - x1*x1 - y1*y1;
    if(t1 >= 0) { t1 *= t1; Vec2 g = grad2(i+i1, j+j1); n1 = t1 * t1 * (g.x*x1 + g.y*y1); }
    float t2 = 0.5f - x2*x2 - y2*y2;
    if(t2 >= 0) { t2 *= t2; Vec2 g = grad2(i+1, j+1); n2 = t2 * t2 * (g.x*x2 + g.y*y2); }

    return 70.0f * (n0 + n1 + n2) + 0.5f;
}

float NoiseGenerator::simplex(float x, float y, float z) const {
    // Simplified 3D simplex
    return gradientNoise(x, y, z); // fallback to gradient for now
}

float NoiseGenerator::ridged(float x, float y, int octaves) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float prev = 1.0f;
    for(int i = 0; i < octaves; i++) {
        float n = gradientNoise(x*frequency, y*frequency);
        n = 1.0f - std::abs(n * 2.0f - 1.0f);
        n = n * n * prev;
        prev = n;
        total += n * amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    return total;
}

} // namespace zv
