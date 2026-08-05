#include "zingvid/scene.h"
#include <cmath>

namespace zv {

void Mesh::computeBounds() {
    if(vertices.empty()) return;
    min = max = vertices[0].position;
    for(const auto& v : vertices) {
        min.x = std::min(min.x, v.position.x);
        min.y = std::min(min.y, v.position.y);
        min.z = std::min(min.z, v.position.z);
        max.x = std::max(max.x, v.position.x);
        max.y = std::max(max.y, v.position.y);
        max.z = std::max(max.z, v.position.z);
    }
    bounds = {min, max};
}

Mat4 Transform::matrix() const {
    Mat4 t = Mat4::translate(position);
    Mat4 rx = Mat4::rotate(rotation.x, {1,0,0});
    Mat4 ry = Mat4::rotate(rotation.y, {0,1,0});
    Mat4 rz = Mat4::rotate(rotation.z, {0,0,1});
    Mat4 s = Mat4::scale(scale);
    return t * (ry * rx * rz) * s;
}

void Entity::updateAnimation(float deltaTime) {
    animTime += deltaTime;
    if(!keyframes.empty()) {
        transform = evaluateKeyframe(animTime);
    }
}

Transform Entity::evaluateKeyframe(float time) const {
    if(keyframes.empty()) return transform;
    if(keyframes.size() == 1) return keyframes[0].second;

    // Find surrounding keyframes
    size_t idx = 0;
    for(size_t i = 0; i < keyframes.size() - 1; i++) {
        if(time >= keyframes[i].first && time <= keyframes[i+1].first) {
            idx = i;
            break;
        }
    }

    const auto& k1 = keyframes[idx];
    const auto& k2 = keyframes[idx+1];
    float t = (time - k1.first) / (k2.first - k1.first);
    t = t * t * (3.0f - 2.0f * t); // smoothstep

    Transform result;
    result.position = lerp(k1.second.position, k2.second.position, t);
    result.rotation = lerp(k1.second.rotation, k2.second.rotation, t);
    result.scale = lerp(k1.second.scale, k2.second.scale, t);
    return result;
}

Entity* Scene::addEntity(EntityType type, const std::string& name) {
    auto e = std::make_unique<Entity>();
    e->name = name;
    e->type = type;
    Entity* ptr = e.get();
    entities.push_back(std::move(e));
    return ptr;
}

void Scene::removeEntity(const std::string& name) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [&name](const auto& e) { return e->name == name; }),
        entities.end());
}

Entity* Scene::findEntity(const std::string& name) {
    for(auto& e : entities) {
        if(e->name == name) return e.get();
    }
    return nullptr;
}

} // namespace zv
