#pragma once

#include <vector>

#include "vulkan_api.h"
#include "vulkan_command_recorder.h"
#include "vulkan_resource.h"

namespace jipu
{

enum class SubmitType
{
    kNone,
    kGraphics,
    kCompute,
    kTransfer,
    kPresent,
};

struct VulkanSubmit
{
    struct Info
    {
        SubmitType type = SubmitType::kNone;

        std::vector<VkCommandBuffer> commandBuffers{};
        std::vector<VkSemaphore> signalSemaphores{};
        std::vector<VkSemaphore> waitSemaphores{};
        std::vector<VkPipelineStageFlags> waitStages{};

        uint32_t swapchainIndex = 0; // for presnet submit type
    } info{};

    struct Object
    {
        struct Resource
        {
            std::unordered_map<VkBuffer, VulkanMemory> buffers{};
            std::unordered_map<VkImage, VulkanMemory> images{};
        };

        std::unordered_set<VkImageView> imageViews{};
        std::unordered_set<VkSampler> samplers{};
        std::unordered_set<VkPipeline> pipelines{};
        std::unordered_set<VkPipelineLayout> pipelineLayouts{};
        std::unordered_set<VkShaderModule> shaderModules{};
        std::unordered_set<VkDescriptorSet> descriptorSet{};
        std::unordered_set<VkDescriptorSetLayout> descriptorSetLayouts{};
        std::unordered_set<VkFramebuffer> framebuffers{};
        std::unordered_set<VkRenderPass> renderPasses{};
        Resource srcResource{};
        Resource dstResource{};
    } object{};

    void add(VkCommandBuffer commandBuffer);
    void addSignalSemaphore(const std::vector<VkSemaphore>& semaphores);
    void addWaitSemaphore(const std::vector<VkSemaphore>& semaphores, const std::vector<VkPipelineStageFlags>& stage);

    void add(VkImageView imageView);
    void add(VkSampler sampler);
    void add(VkPipeline pipeline);
    void add(VkPipelineLayout pipelineLayout);
    void add(const std::vector<VkShaderModule> shaderModules);
    void add(VkDescriptorSet descriptorSet);
    void add(VkDescriptorSetLayout descriptorSetLayout);
    void add(VkFramebuffer framebuffer);
    void add(VkRenderPass renderPass);

    void addSrcBuffer(VulkanBufferResource buffer);
    void addSrcImage(VulkanTextureResource image);
    void addDstBuffer(VulkanBufferResource buffer);
    void addDstImage(VulkanTextureResource image);
};

class VulkanDevice;
class VulkanSubmitContext final
{
public:
    VulkanSubmitContext() = default;
    ~VulkanSubmitContext() = default;

public:
    static VulkanSubmitContext create(VulkanDevice* device, const std::vector<VulkanCommandRecordResult>& results);

public:
    const std::vector<VulkanSubmit>& getSubmits() const;
    std::vector<VulkanSubmit::Info> getSubmitInfos() const;
    std::vector<VulkanSubmit::Object> getSubmitObjects() const;

private:
    std::vector<VulkanSubmit> m_submits{};
};

} // namespace jipu