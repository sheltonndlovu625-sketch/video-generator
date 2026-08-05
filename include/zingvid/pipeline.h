#pragma once
#include "scene.h"
#include <vector>
#include <cstdint>
#include <functional>

namespace zv {

class Renderer;

struct Frame {
    std::vector<uint8_t> pixels; // RGBA
    uint32_t width, height;
    float timestamp;
};

struct PipelineConfig {
    uint32_t width = 640;
    uint32_t height = 360;
    uint32_t fps = 30;
    uint32_t quality = 2; // 1=low, 2=medium, 3=high
    bool useGPU = true;
    bool enableShadows = false; // disabled for mobile
    bool enablePostProcess = true;
};

class Pipeline {
public:
    explicit Pipeline(const PipelineConfig& cfg);
    ~Pipeline();

    void setScene(Scene* scene);
    void renderFrame(float time, Frame& outFrame);
    void renderSequence(std::function<void(const Frame&, int)> callback);

    // Mobile optimization
    void setTileSize(uint32_t size) { tileSize_ = size; }
    void enableEarlyZ(bool enable) { earlyZ_ = enable; }

private:
    PipelineConfig config_;
    Scene* scene_ = nullptr;
    std::unique_ptr<Renderer> renderer_;
    uint32_t tileSize_ = 32; // Mali optimized tile size
    bool earlyZ_ = true;

    void cullEntities(std::vector<Entity*>& visible);
    void sortByDepth(std::vector<Entity*>& entities);
};

} // namespace zv
