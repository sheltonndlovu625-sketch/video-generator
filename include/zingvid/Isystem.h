#pragma once
#include "scene.h"
#include <string>
#include <vector>

namespace zv {

// L-System for procedural plants, coral, structures
class LSystem {
public:
    struct Rule {
        char symbol;
        std::string replacement;
        float probability = 1.0f;
    };

    struct Turtle {
        Vec3 pos;
        Vec3 heading;
        Vec3 up;
        float width;
        Vec4 color;
    };

    void setAxiom(const std::string& axiom);
    void addRule(char sym, const std::string& repl, float prob = 1.0f);
    void setIterations(int n);
    void setStepSize(float s) { stepSize_ = s; }
    void setAngle(float a) { angle_ = a; }
    void setWidth(float w) { baseWidth_ = w; }

    std::string generateString() const;
    Mesh buildMesh(uint32_t seed = 0) const;

    // Presets
    static LSystem treePreset(int variant = 0);
    static LSystem coralPreset(int variant = 0);
    static LSystem fernPreset();

private:
    std::string axiom_;
    std::vector<Rule> rules_;
    int iterations_ = 4;
    float stepSize_ = 1.0f;
    float angle_ = 25.0f;
    float baseWidth_ = 0.1f;
};

} // namespace zv
