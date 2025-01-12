#include "vulkan_pipeline_layout.h"

#include "vulkan_bind_group_layout.h"
#include "vulkan_device.h"

#include "jipu/common/hash.h"
#include <stdexcept>

namespace jipu
{

VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice* device, const PipelineLayoutDescriptor& descriptor)
    : m_device(device)
    , m_descriptor(descriptor)
{
    m_pipelineLayout = m_device->getPipelineLayoutCache()->getVkPipelineLayout(descriptor);
}

VulkanPipelineLayout::~VulkanPipelineLayout()
{
    // do not destroy VkPipelineLayout here. because it is managed by VulkanPipelineLayoutCache.
}

VkPipelineLayout VulkanPipelineLayout::getVkPipelineLayout() const
{
    return m_pipelineLayout;
}

PipelineLayoutInfo VulkanPipelineLayout::getLayoutInfo() const
{
    PipelineLayoutInfo layoutInfo{};
    layoutInfo.bindGroupLayoutInfos.resize(m_descriptor.layouts.size());

    for (uint32_t i = 0; i < layoutInfo.bindGroupLayoutInfos.size(); ++i)
    {
        layoutInfo.bindGroupLayoutInfos[i] = BindGroupLayoutInfo{
            .buffers = downcast(m_descriptor.layouts[i])->getBufferBindingLayouts(),
            .samplers = downcast(m_descriptor.layouts[i])->getSamplerBindingLayouts(),
            .textures = downcast(m_descriptor.layouts[i])->getTextureBindingLayouts(),
        };
    }

    return layoutInfo;
}

// VulkanPipelineLayoutCache

VulkanPipelineLayoutCache::VulkanPipelineLayoutCache(VulkanDevice* device)
    : m_device(device)
{
}

VulkanPipelineLayoutCache::~VulkanPipelineLayoutCache()
{
    clear();
}

VkPipelineLayout VulkanPipelineLayoutCache::getVkPipelineLayout(const PipelineLayoutDescriptor& descriptor)
{
    PipelineLayoutInfo layoutInfo{};
    layoutInfo.bindGroupLayoutInfos.resize(descriptor.layouts.size());

    for (uint32_t i = 0; i < layoutInfo.bindGroupLayoutInfos.size(); ++i)
    {
        layoutInfo.bindGroupLayoutInfos[i] = BindGroupLayoutInfo{
            .buffers = downcast(descriptor.layouts[i])->getBufferBindingLayouts(),
            .samplers = downcast(descriptor.layouts[i])->getSamplerBindingLayouts(),
            .textures = downcast(descriptor.layouts[i])->getTextureBindingLayouts(),
        };
    }

    return getVkPipelineLayout(layoutInfo);
}

VkPipelineLayout VulkanPipelineLayoutCache::getVkPipelineLayout(const PipelineLayoutInfo& layoutInfo)
{
    auto it = m_pipelineLayouts.find(layoutInfo);
    if (it != m_pipelineLayouts.end())
    {
        return it->second;
    }

    std::vector<VkDescriptorSetLayout> layouts{};
    layouts.resize(layoutInfo.bindGroupLayoutInfos.size());
    for (uint32_t i = 0; i < layouts.size(); ++i)
    {
        layouts[i] = m_device->getBindGroupLayoutCache()->getVkDescriptorSetLayout(layoutInfo.bindGroupLayoutInfos[i]);
    }

    VkPipelineLayoutCreateInfo createInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                                           .setLayoutCount = static_cast<uint32_t>(layouts.size()),
                                           .pSetLayouts = layouts.data() };

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkResult result = m_device->vkAPI.CreatePipelineLayout(m_device->getVkDevice(), &createInfo, nullptr, &pipelineLayout);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create VkPipelineLayout");
    }

    m_pipelineLayouts.insert({ layoutInfo, pipelineLayout });

    return pipelineLayout;
}

void VulkanPipelineLayoutCache::clear()
{
    for (auto& [descriptor, pipelineLayout] : m_pipelineLayouts)
    {
        m_device->getDeleter()->safeDestroy(pipelineLayout);
    }

    m_pipelineLayouts.clear();
}

size_t VulkanPipelineLayoutCache::Functor::operator()(const PipelineLayoutInfo& layoutInfo) const
{
    size_t hash = 0;

    for (const auto& bindGroupLayoutInfo : layoutInfo.bindGroupLayoutInfos)
    {
        for (const auto& buffer : bindGroupLayoutInfo.buffers)
        {
            combineHash(hash, buffer.dynamicOffset);
            combineHash(hash, buffer.index);
            combineHash(hash, buffer.stages);
            combineHash(hash, buffer.type);
        }

        for (const auto& sampler : bindGroupLayoutInfo.samplers)
        {
            combineHash(hash, sampler.index);
            combineHash(hash, sampler.stages);
        }

        for (const auto& texture : bindGroupLayoutInfo.textures)
        {
            combineHash(hash, texture.index);
            combineHash(hash, texture.stages);
        }
    }

    return hash;
}

bool VulkanPipelineLayoutCache::Functor::operator()(const PipelineLayoutInfo& lhs,
                                                    const PipelineLayoutInfo& rhs) const
{
    if (lhs.bindGroupLayoutInfos.size() != rhs.bindGroupLayoutInfos.size())
    {
        return false;
    }

    for (auto i = 0; i < lhs.bindGroupLayoutInfos.size(); ++i)
    {
        if (lhs.bindGroupLayoutInfos[i].buffers.size() != rhs.bindGroupLayoutInfos[i].buffers.size() ||
            lhs.bindGroupLayoutInfos[i].samplers.size() != rhs.bindGroupLayoutInfos[i].samplers.size() ||
            lhs.bindGroupLayoutInfos[i].textures.size() != rhs.bindGroupLayoutInfos[i].textures.size())
        {
            return false;
        }

        for (auto j = 0; j < lhs.bindGroupLayoutInfos[i].buffers.size(); ++j)
        {
            if (lhs.bindGroupLayoutInfos[i].buffers[j].dynamicOffset != rhs.bindGroupLayoutInfos[i].buffers[j].dynamicOffset ||
                lhs.bindGroupLayoutInfos[i].buffers[j].index != rhs.bindGroupLayoutInfos[i].buffers[j].index ||
                lhs.bindGroupLayoutInfos[i].buffers[j].stages != rhs.bindGroupLayoutInfos[i].buffers[j].stages ||
                lhs.bindGroupLayoutInfos[i].buffers[j].type != rhs.bindGroupLayoutInfos[i].buffers[j].type)
            {
                return false;
            }
        }

        for (auto j = 0; j < lhs.bindGroupLayoutInfos[i].samplers.size(); ++j)
        {
            if (lhs.bindGroupLayoutInfos[i].samplers[j].index != rhs.bindGroupLayoutInfos[i].samplers[j].index ||
                lhs.bindGroupLayoutInfos[i].samplers[j].stages != rhs.bindGroupLayoutInfos[i].samplers[j].stages)
            {
                return false;
            }
        }

        for (auto j = 0; j < lhs.bindGroupLayoutInfos[i].textures.size(); ++j)
        {
            if (lhs.bindGroupLayoutInfos[i].textures[j].index != rhs.bindGroupLayoutInfos[i].textures[j].index ||
                lhs.bindGroupLayoutInfos[i].textures[j].stages != rhs.bindGroupLayoutInfos[i].textures[j].stages)
            {
                return false;
            }
        }
    }

    return true;
}

} // namespace jipu