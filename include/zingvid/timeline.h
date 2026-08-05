#pragma once
#include "scene.h"
#include <functional>

namespace zv {

struct Keyframe {
    float time;
    Transform transform;
    float easeIn = 0.0f;
    float easeOut = 0.0f;
};

class Timeline {
public:
    void addKeyframe(const std::string& entityName, const Keyframe& kf);
    void update(float currentTime, Scene& scene);
    void clear();
    float duration() const { return duration_; }
    void setDuration(float d) { duration_ = d; }

private:
    std::unordered_map<std::string, std::vector<Keyframe>> tracks_;
    float duration_ = 5.0f;
    Transform interpolate(const Keyframe& a, const Keyframe& b, float t) const;
};

} // namespace zv
