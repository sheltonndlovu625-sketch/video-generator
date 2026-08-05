#pragma once
#include "math.h"
#include <vector>
#include <string>
#include <memory>
#include <cstdint>

namespace zv {

enum class EntityType {
    Human,
    Animal,
    Place,
    Prop,
    ParticleSystem
};

enum class AnimationType {
    Idle,
    Walk,
    Run,
    Gesture,
    Custom
};

struct Material {
    Vec3 albedo{1,1,1};
    float roughness = 0.5f;
    float metallic = 0.0f;
    float emission = 0.0f;
};

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec4 color{1,1,1,1};
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Material material;
    AABB bounds;
    void computeBounds();
};

struct Transform {
    Vec3 position{0,0,0};
    Vec3 rotation{0,0,0};
    Vec3 scale{1,1,1};
    Mat4 matrix() const;
};

struct Entity {
    std::string name;
    EntityType type;
    Transform transform;
    std::vector<Mesh> meshes;
    AnimationType anim = AnimationType::Idle;
    float animTime = 0.0f;
    std::vector<std::pair<float, Transform>> keyframes;
    uint32_t procSeed = 0;
    float procScale = 1.0f;
    int procDetail = 3;

    void updateAnimation(float deltaTime);
    Transform evaluateKeyframe(float time) const;
};

struct Camera {
    Vec3 position{0, 5, 10};
    Vec3 target{0, 0, 0};
    Vec3 up{0, 1, 0};
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
};

struct Light {
    Vec3 position;
    Vec3 color{1,1,1};
    float intensity = 1.0f;
    float range = 50.0f;
};

struct Scene {
    std::vector<std::unique_ptr<Entity>> entities;
    Camera camera;
    std::vector<Light> lights;
    Vec3 ambientLight{0.1f, 0.1f, 0.15f};
    float duration = 5.0f;
    uint32_t fps = 30;
    uint32_t width = 640;
    uint32_t height = 360;

    Entity* addEntity(EntityType type, const std::string& name);
    void removeEntity(const std::string& name);
    Entity* findEntity(const std::string& name);
};

} // namespace zv
