#pragma once

#include "jipu/binding_group.h"
#include "utils/cast.h"
#include "vulkan_api.h"

namespace jipu
{

class VulkanDevice;
class VulkanBindingGroup : public BindingGroup
{
public:
    VulkanBindingGroup() = delete;
    VulkanBindingGroup(VulkanDevice* device, const BindingGroupDescriptor& descriptor);
    ~VulkanBindingGroup() override;

    VkDescriptorSet getVkDescriptorSet() const;

private:
    VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;

private:
    VulkanDevice* m_device = nullptr;
    const BindingGroupDescriptor m_descriptor{};
};
DOWN_CAST(VulkanBindingGroup, BindingGroup);
} // namespace jipu