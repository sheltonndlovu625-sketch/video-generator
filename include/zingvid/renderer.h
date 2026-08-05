#pragma once
#include "scene.h"
#include "pipeline.h"
#include <vector>
#include <cstdint>

namespace zv {

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual bool initialize(uint32_t width, uint32_t height) = 0;
    virtual void shutdown() = 0;
    virtual void render(const Scene& scene, float time, std::vector<uint8_t>& outPixels) = 0;
    virtual bool isGPU() const = 0;
    virtual std::string name() const = 0;
};

// Software renderer fallback - works everywhere
class SoftwareRenderer : public Renderer {
public:
    SoftwareRenderer();
    ~SoftwareRenderer() override;
    bool initialize(uint32_t width, uint32_t height) override;
    void shutdown() override;
    void render(const Scene& scene, float time, std::vector<uint8_t>& outPixels) override;
    bool isGPU() const override { return false; }
    std::string name() const override { return "Software"; }

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// OpenGL ES renderer for mobile
class GLESRenderer : public Renderer {
public:
    GLESRenderer();
    ~GLESRenderer() override;
    bool initialize(uint32_t width, uint32_t height) override;
    void shutdown() override;
    void render(const Scene& scene, float time, std::vector<uint8_t>& outPixels) override;
    bool isGPU() const override { return true; }
    std::string name() const override { return "OpenGL_ES"; }

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace zv
