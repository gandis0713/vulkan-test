#include "vulkan_device.h"

#include "utils/log.h"
#include "vulkan_adapter.h"
#include "vulkan_driver.h"
#include "vulkan_framebuffer.h"
#include "vulkan_pipeline.h"
#include "vulkan_queue.h"
#include "vulkan_render_pass.h"
#include "vulkan_swapchain.h"

#include <stdexcept>
const std::vector<const char*> getRequiredDeviceExtension()
{
    std::vector<const char*> requiredDeviceExtension;

    requiredDeviceExtension.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    LOG_INFO("Required Device extensions :");
    for (const auto& extension : requiredDeviceExtension)
    {
        LOG_INFO("{}{}", '\t', extension);
    }
    return requiredDeviceExtension;
};

namespace vkt
{

VulkanDevice::VulkanDevice(VulkanAdapter* adapter, DeviceDescriptor descriptor)
    : Device(adapter, descriptor)
    , m_frameBufferCache(this)
    , m_renderPassCache(this)
    , vkAPI(static_cast<VulkanDriver*>(adapter->getDriver())->vkAPI)
{
    const VulkanDeviceInfo& info = adapter->getDeviceInfo();
    constexpr uint32_t queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

    std::unordered_set<uint32_t> queueFamilyIndices{};
    for (uint32_t i = 0; i < info.queueFamilyProperties.size(); ++i)
    {
        if ((info.queueFamilyProperties[i].queueFlags & queueFlags) ==
            queueFlags)
        {
            queueFamilyIndices.insert(i);
        }
    }

    createDevice(queueFamilyIndices);

    VulkanDeviceKnobs deviceKnobs{ true }; // TODO: generate deviceKnobs.
    vkAPI.loadDeviceProcs(m_device, deviceKnobs);

    for (const uint32_t& index : queueFamilyIndices)
    {
        VkQueue queue{};
        vkAPI.GetDeviceQueue(m_device, index, 0, &queue);
        m_queues.push_back(queue);
    }
}

VulkanDevice::~VulkanDevice()
{
    LOG_TRACE(__func__);

    m_frameBufferCache.clear();
    m_renderPassCache.clear();

    vkAPI.DestroyDevice(m_device, nullptr);
}

std::unique_ptr<SwapChain> VulkanDevice::createSwapChain(const SwapChainDescriptor& descriptor)
{
    return std::make_unique<VulkanSwapChain>(this, std::move(descriptor));
}

std::unique_ptr<Pipeline> VulkanDevice::createPipeline(PipelineDescriptor descriptor)
{
    return std::make_unique<VulkanPipeline>(this, descriptor);
}

std::unique_ptr<Queue> VulkanDevice::createQueue(const QueueDescriptor& descriptor)
{
    return std::make_unique<VulkanQueue>(this, descriptor);
}

VulkanRenderPass* VulkanDevice::getRenderPass(const VulkanRenderPassDescriptor& descriptor)
{
    return m_renderPassCache.getRenderPass(descriptor);
}

VulkanFrameBuffer* VulkanDevice::getFrameBuffer(const VulkanFramebufferDescriptor& descriptor)
{
    return m_frameBufferCache.getFrameBuffer(descriptor);
}

VkDevice VulkanDevice::getDevice() const
{
    return m_device;
}

VkPhysicalDevice VulkanDevice::getPhysicalDevice() const
{
    VulkanAdapter* vulkanAdapter = static_cast<VulkanAdapter*>(m_adapter);

    return vulkanAdapter->getPhysicalDevice();
}

VkQueue VulkanDevice::getQueue() const
{
    // TODO: return suit queue
    if (!m_queues.empty())
    {
        return m_queues[0];
    }

    return VK_NULL_HANDLE;
}

uint32_t VulkanDevice::getQueueIndex() const
{
    // TODO: return suit queue index
    if (!m_queues.empty())
    {
        return 3;
    }

    throw std::runtime_error("There is no queue in device.");

    return 0;
}

void VulkanDevice::createDevice(const std::unordered_set<uint32_t>& queueFamilyIndices)
{
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

    float queuePriority = 1.0f;
    for (const uint32_t queueFamilyIndex : queueFamilyIndices)
    {
        VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
        deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
    }

    auto vulkanAdapter = static_cast<VulkanAdapter*>(m_adapter);

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &vulkanAdapter->getDeviceInfo().physicalDeviceFeatures;

    std::vector<const char*> requiredDeviceExtensions = getRequiredDeviceExtension();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();

    // TODO: validation layer.
    // // set validation layers to be compatible with older implementations:
    // if (enableValidationLayers)
    // {
    //     const std::vector<const char*>& requiredValidationLayers = getRequiredValidationLayers();
    //     if (enableValidationLayers && !checkValidationLayerSupport(requiredValidationLayers))
    //     {
    //         throw std::runtime_error("validation layers requested, but not "
    //                                  "available for device!");
    //     }
    //     deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
    //     deviceCreateInfo.ppEnabledLayerNames = requiredValidationLayers.data();
    // }
    // else
    // {
    //     deviceCreateInfo.enabledLayerCount = 0;
    // }

    VkPhysicalDevice physicalDevice = static_cast<VulkanAdapter*>(m_adapter)->getPhysicalDevice();
    if (vkAPI.CreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }
}

void VulkanDevice::gatherQueues()
{
    // for (const uint32_t& index : queueFamilyIndices)
    // {
    //     VkQueue queue{};
    //     vkAPI.GetDeviceQueue(m_device, index, 0, &queue);
    //     m_queues.push_back(queue);
    // }
}

} // namespace vkt
