#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "file.h"
#include "im_gui.h"
#include "light.h"
#include "model.h"
#include "sample.h"

#include "jipu/buffer.h"
#include "jipu/command_buffer.h"
#include "jipu/command_encoder.h"
#include "jipu/device.h"
#include "jipu/driver.h"
#include "jipu/physical_device.h"
#include "jipu/pipeline_layout.h"
#include "jipu/queue.h"
#include "jipu/render_pass_encoder.h"
#include "jipu/surface.h"
#include "jipu/swapchain.h"

#include "vulkan_pipeline.h"

#include "khronos_texture.h"

namespace jipu
{

class VulkanFramebuffer;
class VulkanRenderPass;
class VulkanNBufferingSample : public Sample, public Im_Gui
{
public:
    VulkanNBufferingSample() = delete;
    VulkanNBufferingSample(const SampleDescriptor& descriptor);
    ~VulkanNBufferingSample() override;

public:
    void init() override;
    void update() override;
    void draw() override;

private:
    void updateImGui() override;

private:
    void createDriver();
    void getPhysicalDevices();
    void createSurface();
    void createDevice();
    void createSwapchain();
    void createCommandBuffer();
    void createQueue();

    void createDepthStencilTexture();
    void createDepthStencilTextureView();

    void createOffscreenPositionColorAttachmentTexture();
    void createOffscreenPositionColorAttachmentTextureView();
    void createOffscreenNormalColorAttachmentTexture();
    void createOffscreenNormalColorAttachmentTextureView();
    void createOffscreenAlbedoColorAttachmentTexture();
    void createOffscreenAlbedoColorAttachmentTextureView();
    void createOffscreenColorMapTexture();
    void createOffscreenColorMapTextureView();
    void createOffscreenNormalMapTexture();
    void createOffscreenNormalMapTextureView();
    void createOffscreenCamera();
    void createOffscreenUniformBuffer();
    void createOffscreenVertexBuffer();
    void createOffscreenBindingGroupLayout();
    void createOffscreenBindingGroup();
    void createOffscreenPipelineLayout();
    void createOffscreenPipeline();

    void updateOffscreenUniformBuffer();

    void createCompositionBindingGroupLayout();
    void createCompositionBindingGroup();
    void createCompositionPipelineLayout();
    void createCompositionPipeline();
    void createCompositionUniformBuffer();
    void createCompositionVertexBuffer();

    void updateCompositionUniformBuffer();

    void recreateSwapchain();

private:
    std::unique_ptr<Driver> m_driver = nullptr;
    std::vector<std::unique_ptr<PhysicalDevice>> m_physicalDevices{};
    std::unique_ptr<Surface> m_surface = nullptr;
    std::unique_ptr<Device> m_device = nullptr;
    std::unique_ptr<Swapchain> m_swapchain = nullptr;

    struct CompositionUBO
    {
        struct Light
        {
            alignas(16) glm::vec3 position;
            alignas(16) glm::vec3 color;
        };

        std::vector<CompositionUBO::Light> lights{};
        alignas(16) glm::vec3 cameraPosition;
        int lightCount = 8;
        int showTexture = 0;
        int padding1;
        int padding2;
    };

    struct MVP
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } m_mvp;

    struct
    {
        std::unique_ptr<Texture> positionColorAttachmentTexture = nullptr;
        std::unique_ptr<TextureView> positionColorAttachmentTextureView = nullptr;
        std::unique_ptr<Texture> normalColorAttachmentTexture = nullptr;
        std::unique_ptr<TextureView> normalColorAttachmentTextureView = nullptr;
        std::unique_ptr<Texture> albedoColorAttachmentTexture = nullptr;
        std::unique_ptr<TextureView> albedoColorAttachmentTextureView = nullptr;
        std::vector<std::unique_ptr<BindingGroupLayout>> bindingGroupLayouts{};
        std::vector<std::unique_ptr<BindingGroup>> bindingGroups{};
        std::unique_ptr<ShaderModule> vertexShaderModule = nullptr;
        std::unique_ptr<ShaderModule> fragmentShaderModule = nullptr;
        std::unique_ptr<PipelineLayout> pipelineLayout = nullptr;
        std::unique_ptr<RenderPipeline> renderPipeline = nullptr;

        std::unique_ptr<Texture> colorMapTexture = nullptr;
        std::unique_ptr<TextureView> colorMapTextureView = nullptr;
        std::unique_ptr<Texture> normalMapTexture = nullptr;
        std::unique_ptr<TextureView> normalMapTextureView = nullptr;
        std::unique_ptr<Sampler> colorMapSampler = nullptr;
        std::unique_ptr<Sampler> normalMapSampler = nullptr;
        std::unique_ptr<Buffer> uniformBuffer = nullptr;
        std::unique_ptr<Buffer> vertexBuffer = nullptr;
        std::unique_ptr<Buffer> indexBuffer = nullptr;
        std::unique_ptr<Camera> camera = nullptr;
        Polygon polygon{};
    } m_offscreen;

    struct CompositionVertex
    {
        glm::vec3 position;
        glm::vec2 textureCoordinate;
    };
    struct
    {
        std::unique_ptr<Sampler> positionSampler = nullptr;
        std::unique_ptr<Sampler> normalSampler = nullptr;
        std::unique_ptr<Sampler> albedoSampler = nullptr;
        std::vector<std::unique_ptr<BindingGroupLayout>> bindingGroupLayouts{};
        std::vector<std::unique_ptr<BindingGroup>> bindingGroups{};
        std::unique_ptr<ShaderModule> vertexShaderModule = nullptr;
        std::unique_ptr<ShaderModule> fragmentShaderModule = nullptr;
        std::unique_ptr<PipelineLayout> pipelineLayout = nullptr;
        std::unique_ptr<RenderPipeline> renderPipeline = nullptr;

        std::unique_ptr<Buffer> uniformBuffer = nullptr;
        std::unique_ptr<Buffer> vertexBuffer = nullptr;
        CompositionUBO ubo{};
        std::vector<CompositionVertex> vertices{
            { { -1.0, -1.0, 0.0 }, { 0.0, 0.0 } },
            { { -1.0, 1.0, 0.0 }, { 0.0, 1.0 } },
            { { 1.0, -1.0, 0.0 }, { 1.0, 0.0 } },
            { { 1.0, -1.0, 0.0 }, { 1.0, 0.0 } },
            { { -1.0, 1.0, 0.0 }, { 0.0, 1.0 } },
            { { 1.0, 1.0, 0.0 }, { 1.0, 1.0 } },
        };
    } m_composition;

    std::unique_ptr<CommandBuffer> m_commandBuffer = nullptr;
    std::unique_ptr<Queue> m_queue = nullptr;
    std::unique_ptr<Texture> m_depthStencilTexture = nullptr;
    std::unique_ptr<TextureView> m_depthStencilTextureView = nullptr;

    uint32_t m_sampleCount = 1;
    int m_lightMax = 10;
    VkPresentModeKHR m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t m_minImageCount = 2;
};

} // namespace jipu