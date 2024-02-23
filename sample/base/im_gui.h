
#pragma once

#include <chrono>
#include <glm/glm.hpp>
#include <imgui.h>

#include "jipu/buffer.h"
#include "jipu/command_buffer.h"
#include "jipu/command_encoder.h"
#include "jipu/device.h"
#include "jipu/driver.h"
#include "jipu/physical_device.h"
#include "jipu/pipeline.h"
#include "jipu/pipeline_layout.h"
#include "jipu/queue.h"
#include "jipu/surface.h"
#include "jipu/swapchain.h"

namespace jipu
{

class Im_Gui
{
protected:
    void initImGui(Device* device, Queue* queue, Swapchain* swapchain);
    void clearImGui();
    virtual void updateImGui() = 0;
    void buildImGui();
    void drawImGui(CommandEncoder* commandEncoder, TextureView* renderView);

protected:
    struct Padding
    {
        float top = 0.0f;
        float bottom = 0.0f;
    } m_padding;

private:
    Device* m_device = nullptr;
    Queue* m_queue = nullptr;
    Swapchain* m_swapchain = nullptr;

    struct UITransform
    {
        glm::vec2 scale = { 0, 0 };
        glm::vec2 translate = { 0, 0 };
    } m_uiTransform;

    std::unique_ptr<Buffer> m_vertexBuffer = nullptr;
    std::unique_ptr<Buffer> m_indexBuffer = nullptr;
    std::unique_ptr<Buffer> m_uniformBuffer = nullptr;
    std::unique_ptr<Texture> m_fontTexture = nullptr;
    std::unique_ptr<TextureView> m_fontTextureView = nullptr;
    std::unique_ptr<Sampler> m_fontSampler = nullptr;
    std::vector<std::unique_ptr<BindingGroupLayout>> m_bindingGroupLayouts{};
    std::vector<std::unique_ptr<BindingGroup>> m_bindingGroups{};
    std::unique_ptr<PipelineLayout> m_pipelineLayout = nullptr;
    std::unique_ptr<RenderPipeline> m_pipeline = nullptr;

protected:
    void debugWindow();

private:
    struct FPS
    {
        std::chrono::milliseconds time = std::chrono::milliseconds::zero();
        uint64_t frame = 0;
        float fps = 0.0f;
    } m_fps;
    void updateFPS();
};

} // namespace jipu