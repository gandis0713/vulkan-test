
#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

#include "vkt/gpu/buffer.h"
#include "vkt/gpu/command_buffer.h"
#include "vkt/gpu/command_encoder.h"
#include "vkt/gpu/device.h"
#include "vkt/gpu/driver.h"
#include "vkt/gpu/physical_device.h"
#include "vkt/gpu/pipeline.h"
#include "vkt/gpu/pipeline_layout.h"
#include "vkt/gpu/queue.h"
#include "vkt/gpu/surface.h"
#include "vkt/gpu/swapchain.h"

namespace vkt
{

class Im_Gui
{
protected:
    virtual void initImGui(Device* device, Queue* queue, Swapchain* swapchain);
    virtual void setupImGui() = 0;
    virtual void updateImGui();
    virtual void drawImGui(RenderPassEncoder* renderPassEncoder);
    virtual void clearImGui();

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
    std::unique_ptr<BindingGroupLayout> m_bindingGroupLayout = nullptr;
    std::unique_ptr<BindingGroup> m_bindingGroup = nullptr;
    std::unique_ptr<PipelineLayout> m_pipelineLayout = nullptr;
    std::unique_ptr<Pipeline> m_pipeline = nullptr;
};

} // namespace vkt