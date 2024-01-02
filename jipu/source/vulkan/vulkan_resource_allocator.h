#pragma once

#include "vulkan_resource.h"

namespace jipu
{

struct VulkanResourceAllocatorDescriptor
{
};

class VulkanDevice;
class VulkanResourceAllocator final
{
public:
    VulkanResourceAllocator() = delete;
    VulkanResourceAllocator(VulkanDevice* device, const VulkanResourceAllocatorDescriptor& descriptor);
    ~VulkanResourceAllocator();

    VulkanBufferResource createBuffer(const VkBufferCreateInfo& createInfo);
    void destroyBuffer(const VulkanBufferResource& bufferResource);

    VulkanTextureResource createTexture(const VkImageCreateInfo& createInfo);
    void destroyTexture(VulkanTextureResource textureResource);

    void* map(VulkanAllocation allocation);
    void unmap(VulkanAllocation allocation);

private:
    VulkanDevice* m_device = nullptr;
#if defined(USE_VMA)
    VmaAllocator m_allocator = VK_NULL_HANDLE;
    VmaVulkanFunctions m_vmaFunctions{};
#endif
};

//

} // namespace jipu