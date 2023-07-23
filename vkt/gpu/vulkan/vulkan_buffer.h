#include "utils/cast.h"
#include "vkt/gpu/buffer.h"
#include "vulkan_api.h"
#include "vulkan_memory.h"

#include <memory>

namespace vkt
{

class VulkanDevice;
class VulkanBuffer : public Buffer
{
public:
    VulkanBuffer() = delete;
    VulkanBuffer(VulkanDevice* device, const BufferDescriptor& descriptor) noexcept(false);
    ~VulkanBuffer() override;

    void* map() override;
    void unmap() override;

    VkBuffer getVkBuffer() const;

private:
    VkBuffer m_buffer = VK_NULL_HANDLE;
    std::unique_ptr<VulkanMemory> m_memory = nullptr;

    void* m_mappedPtr = nullptr;
};

DOWN_CAST(VulkanBuffer, Buffer);

// Convert Helper
VkAccessFlags ToVkAccessFlags(BufferUsageFlags flags);
VkBufferUsageFlags ToVkBufferUsageFlags(BufferUsageFlags usages);

// TODO: remove or remain.
// BufferUsageFlags ToBufferUsageFlags(VkAccessFlags vkflags);
// BufferUsageFlags ToBufferUsageFlags(VkBufferUsageFlags usages);
} // namespace vkt
