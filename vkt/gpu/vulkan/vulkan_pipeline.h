#pragma once

#include "utils/cast.h"
#include "vkt/gpu/pipeline.h"
#include "vulkan_api.h"

#include <string>
#include <vector>

namespace vkt
{

class VulkanDevice;
class VulkanRenderPass;

class VKT_EXPORT VulkanPipeline : public Pipeline
{
public:
    VulkanPipeline() = delete;
    VulkanPipeline(VulkanDevice* device, const PipelineDescriptor& descriptor);
    ~VulkanPipeline() override = default;

    VulkanPipeline(const VulkanPipeline&) = delete;
    VulkanPipeline& operator=(const VulkanPipeline&) = delete;

    VkPipeline getVkPipeline() const;

    void setRenderPass(VulkanRenderPass* renderPass);

    void destroy();

    void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath);
    VkShaderModule createShaderModule(const std::vector<char>& codeBuffer);

private:
    // shader module
    VkShaderModule m_vertShaderModule{ VK_NULL_HANDLE };
    VkShaderModule m_fragShaderModule{ VK_NULL_HANDLE };

    // pipeline
    VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
    VkPipeline m_graphicsPipeline{ VK_NULL_HANDLE };

    VulkanRenderPass* m_renderPass{ nullptr };
};

DOWN_CAST(VulkanPipeline, Pipeline);

} // namespace vkt