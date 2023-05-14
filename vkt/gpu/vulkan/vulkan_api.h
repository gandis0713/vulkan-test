#pragma once

#if defined(__linux__)
    #define VK_USE_PLATFORM_XCB_KHR
#elif defined(__APPLE__)
    // #define VK_USE_PLATFORM_METAL_EXT
    #define VK_USE_PLATFORM_MACOS_MVK
#elif defined(WIN32)
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_NO_PROTOTYPES 1
#include <vulkan/vulkan.h>

namespace vkt
{

class DynamicLib;

struct VulkanDriverKnobs
{
    uint32_t apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

    // TODO: use bitset instead of bool type.
    bool debugReport = false;
    bool surface = false;
    bool macosSurface = false;
    bool metalSurface = false;
    bool win32Surface = false;
};

struct VulkanDeviceKnobs
{
    bool swapchain = false;
};

/// @brief ref: https://dawn.googlesource.com/dawn/+/refs/heads/main/src/dawn/native/vulkan/ VulkanAPI.h
struct VulkanAPI
{
    bool loadDriverProcs(DynamicLib* vulkanLib);
    bool loadInstanceProcs(VkInstance instance, const VulkanDriverKnobs& globalInfo);
    bool loadDeviceProcs(VkDevice device, const VulkanDeviceKnobs& deviceKnobs);

    // ---------- Driver procs

    // Initial proc from which we can get all the others
    PFN_vkGetInstanceProcAddr GetInstanceProcAddr = nullptr;

    PFN_vkCreateInstance CreateInstance = nullptr;
    PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties = nullptr;
    PFN_vkEnumerateInstanceLayerProperties EnumerateInstanceLayerProperties = nullptr;
    // DestroyInstance isn't technically a global proc but we want to be able to use it
    // before querying the instance procs in case we need to error out during initialization.
    PFN_vkDestroyInstance DestroyInstance = nullptr;

    // Core Vulkan 1.1
    PFN_vkEnumerateInstanceVersion EnumerateInstanceVersion = nullptr;

    // ---------- Instance procs

    // Core Vulkan 1.0
    PFN_vkCreateDevice CreateDevice = nullptr;
    PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties = nullptr;
    PFN_vkEnumerateDeviceLayerProperties EnumerateDeviceLayerProperties = nullptr;
    PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices = nullptr;
    PFN_vkGetDeviceProcAddr GetDeviceProcAddr = nullptr;
    PFN_vkGetPhysicalDeviceFeatures GetPhysicalDeviceFeatures = nullptr;
    PFN_vkGetPhysicalDeviceFormatProperties GetPhysicalDeviceFormatProperties = nullptr;
    PFN_vkGetPhysicalDeviceImageFormatProperties GetPhysicalDeviceImageFormatProperties =
        nullptr;
    PFN_vkGetPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties = nullptr;
    PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties = nullptr;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties =
        nullptr;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties
        GetPhysicalDeviceSparseImageFormatProperties = nullptr;
    // Not technically an instance proc but we want to be able to use it as soon as the
    // device is created.
    PFN_vkDestroyDevice DestroyDevice = nullptr;

    // VK_EXT_debug_report
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallbackEXT = nullptr;
    PFN_vkDebugReportMessageEXT DebugReportMessageEXT = nullptr;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallbackEXT = nullptr;

    // VK_KHR_surface
    PFN_vkDestroySurfaceKHR DestroySurfaceKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesKHR =
        nullptr;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsKHR = nullptr;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesKHR =
        nullptr;

    // Core Vulkan 1.1 promoted extensions

    // VK_KHR_external_memory_capabilities
    PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR
        GetPhysicalDeviceExternalBufferPropertiesKHR = nullptr;

    // VK_KHR_external_semaphore_capabilities
    PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR
        GetPhysicalDeviceExternalSemaphorePropertiesKHR = nullptr;

    // VK_KHR_get_physical_device_properties2
    PFN_vkGetPhysicalDeviceFeatures2KHR GetPhysicalDeviceFeatures2KHR = nullptr;
    PFN_vkGetPhysicalDeviceProperties2KHR GetPhysicalDeviceProperties2KHR = nullptr;
    PFN_vkGetPhysicalDeviceFormatProperties2KHR GetPhysicalDeviceFormatProperties2KHR = nullptr;
    PFN_vkGetPhysicalDeviceImageFormatProperties2KHR
        GetPhysicalDeviceImageFormatProperties2KHR = nullptr;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR
        GetPhysicalDeviceQueueFamilyProperties2KHR = nullptr;
    PFN_vkGetPhysicalDeviceMemoryProperties2KHR GetPhysicalDeviceMemoryProperties2KHR = nullptr;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR
        GetPhysicalDeviceSparseImageFormatProperties2KHR = nullptr;

#ifdef VK_USE_PLATFORM_FUCHSIA
    // FUCHSIA_image_pipe_surface
    PFN_vkCreateImagePipeSurfaceFUCHSIA CreateImagePipeSurfaceFUCHSIA = nullptr;
#endif

    // ---------- Device procs

    // Core Vulkan 1.0
    PFN_vkAllocateCommandBuffers AllocateCommandBuffers = nullptr;
    PFN_vkAllocateDescriptorSets AllocateDescriptorSets = nullptr;
    PFN_vkAllocateMemory AllocateMemory = nullptr;
    PFN_vkBeginCommandBuffer BeginCommandBuffer = nullptr;
    PFN_vkBindBufferMemory BindBufferMemory = nullptr;
    PFN_vkBindImageMemory BindImageMemory = nullptr;
    PFN_vkCmdBeginQuery CmdBeginQuery = nullptr;
    PFN_vkCmdBeginRenderPass CmdBeginRenderPass = nullptr;
    PFN_vkCmdBindDescriptorSets CmdBindDescriptorSets = nullptr;
    PFN_vkCmdBindIndexBuffer CmdBindIndexBuffer = nullptr;
    PFN_vkCmdBindPipeline CmdBindPipeline = nullptr;
    PFN_vkCmdBindVertexBuffers CmdBindVertexBuffers = nullptr;
    PFN_vkCmdBlitImage CmdBlitImage = nullptr;
    PFN_vkCmdClearAttachments CmdClearAttachments = nullptr;
    PFN_vkCmdClearColorImage CmdClearColorImage = nullptr;
    PFN_vkCmdClearDepthStencilImage CmdClearDepthStencilImage = nullptr;
    PFN_vkCmdCopyBuffer CmdCopyBuffer = nullptr;
    PFN_vkCmdCopyBufferToImage CmdCopyBufferToImage = nullptr;
    PFN_vkCmdCopyImage CmdCopyImage = nullptr;
    PFN_vkCmdCopyImageToBuffer CmdCopyImageToBuffer = nullptr;
    PFN_vkCmdCopyQueryPoolResults CmdCopyQueryPoolResults = nullptr;
    PFN_vkCmdDispatch CmdDispatch = nullptr;
    PFN_vkCmdDispatchIndirect CmdDispatchIndirect = nullptr;
    PFN_vkCmdDraw CmdDraw = nullptr;
    PFN_vkCmdDrawIndexed CmdDrawIndexed = nullptr;
    PFN_vkCmdDrawIndexedIndirect CmdDrawIndexedIndirect = nullptr;
    PFN_vkCmdDrawIndirect CmdDrawIndirect = nullptr;
    PFN_vkCmdEndQuery CmdEndQuery = nullptr;
    PFN_vkCmdEndRenderPass CmdEndRenderPass = nullptr;
    PFN_vkCmdExecuteCommands CmdExecuteCommands = nullptr;
    PFN_vkCmdFillBuffer CmdFillBuffer = nullptr;
    PFN_vkCmdNextSubpass CmdNextSubpass = nullptr;
    PFN_vkCmdPipelineBarrier CmdPipelineBarrier = nullptr;
    PFN_vkCmdPushConstants CmdPushConstants = nullptr;
    PFN_vkCmdResetEvent CmdResetEvent = nullptr;
    PFN_vkCmdResetQueryPool CmdResetQueryPool = nullptr;
    PFN_vkCmdResolveImage CmdResolveImage = nullptr;
    PFN_vkCmdSetBlendConstants CmdSetBlendConstants = nullptr;
    PFN_vkCmdSetDepthBias CmdSetDepthBias = nullptr;
    PFN_vkCmdSetDepthBounds CmdSetDepthBounds = nullptr;
    PFN_vkCmdSetEvent CmdSetEvent = nullptr;
    PFN_vkCmdSetLineWidth CmdSetLineWidth = nullptr;
    PFN_vkCmdSetScissor CmdSetScissor = nullptr;
    PFN_vkCmdSetStencilCompareMask CmdSetStencilCompareMask = nullptr;
    PFN_vkCmdSetStencilReference CmdSetStencilReference = nullptr;
    PFN_vkCmdSetStencilWriteMask CmdSetStencilWriteMask = nullptr;
    PFN_vkCmdSetViewport CmdSetViewport = nullptr;
    PFN_vkCmdUpdateBuffer CmdUpdateBuffer = nullptr;
    PFN_vkCmdWaitEvents CmdWaitEvents = nullptr;
    PFN_vkCmdWriteTimestamp CmdWriteTimestamp = nullptr;
    PFN_vkCreateBuffer CreateBuffer = nullptr;
    PFN_vkCreateBufferView CreateBufferView = nullptr;
    PFN_vkCreateCommandPool CreateCommandPool = nullptr;
    PFN_vkCreateComputePipelines CreateComputePipelines = nullptr;
    PFN_vkCreateDescriptorPool CreateDescriptorPool = nullptr;
    PFN_vkCreateDescriptorSetLayout CreateDescriptorSetLayout = nullptr;
    PFN_vkCreateEvent CreateEvent = nullptr;
    PFN_vkCreateFence CreateFence = nullptr;
    PFN_vkCreateFramebuffer CreateFramebuffer = nullptr;
    PFN_vkCreateGraphicsPipelines CreateGraphicsPipelines = nullptr;
    PFN_vkCreateImage CreateImage = nullptr;
    PFN_vkCreateImageView CreateImageView = nullptr;
    PFN_vkCreatePipelineCache CreatePipelineCache = nullptr;
    PFN_vkCreatePipelineLayout CreatePipelineLayout = nullptr;
    PFN_vkCreateQueryPool CreateQueryPool = nullptr;
    PFN_vkCreateRenderPass CreateRenderPass = nullptr;
    PFN_vkCreateSampler CreateSampler = nullptr;
    PFN_vkCreateSemaphore CreateSemaphore = nullptr;
    PFN_vkCreateShaderModule CreateShaderModule = nullptr;
    PFN_vkDestroyBuffer DestroyBuffer = nullptr;
    PFN_vkDestroyBufferView DestroyBufferView = nullptr;
    PFN_vkDestroyCommandPool DestroyCommandPool = nullptr;
    PFN_vkDestroyDescriptorPool DestroyDescriptorPool = nullptr;
    PFN_vkDestroyDescriptorSetLayout DestroyDescriptorSetLayout = nullptr;
    PFN_vkDestroyEvent DestroyEvent = nullptr;
    PFN_vkDestroyFence DestroyFence = nullptr;
    PFN_vkDestroyFramebuffer DestroyFramebuffer = nullptr;
    PFN_vkDestroyImage DestroyImage = nullptr;
    PFN_vkDestroyImageView DestroyImageView = nullptr;
    PFN_vkDestroyPipeline DestroyPipeline = nullptr;
    PFN_vkDestroyPipelineCache DestroyPipelineCache = nullptr;
    PFN_vkDestroyPipelineLayout DestroyPipelineLayout = nullptr;
    PFN_vkDestroyQueryPool DestroyQueryPool = nullptr;
    PFN_vkDestroyRenderPass DestroyRenderPass = nullptr;
    PFN_vkDestroySampler DestroySampler = nullptr;
    PFN_vkDestroySemaphore DestroySemaphore = nullptr;
    PFN_vkDestroyShaderModule DestroyShaderModule = nullptr;
    PFN_vkDeviceWaitIdle DeviceWaitIdle = nullptr;
    PFN_vkEndCommandBuffer EndCommandBuffer = nullptr;
    PFN_vkFlushMappedMemoryRanges FlushMappedMemoryRanges = nullptr;
    PFN_vkFreeCommandBuffers FreeCommandBuffers = nullptr;
    PFN_vkFreeDescriptorSets FreeDescriptorSets = nullptr;
    PFN_vkFreeMemory FreeMemory = nullptr;
    PFN_vkGetBufferMemoryRequirements GetBufferMemoryRequirements = nullptr;
    PFN_vkGetDeviceMemoryCommitment GetDeviceMemoryCommitment = nullptr;
    PFN_vkGetDeviceQueue GetDeviceQueue = nullptr;
    PFN_vkGetEventStatus GetEventStatus = nullptr;
    PFN_vkGetFenceStatus GetFenceStatus = nullptr;
    PFN_vkGetImageMemoryRequirements GetImageMemoryRequirements = nullptr;
    PFN_vkGetImageSparseMemoryRequirements GetImageSparseMemoryRequirements = nullptr;
    PFN_vkGetImageSubresourceLayout GetImageSubresourceLayout = nullptr;
    PFN_vkGetPipelineCacheData GetPipelineCacheData = nullptr;
    PFN_vkGetQueryPoolResults GetQueryPoolResults = nullptr;
    PFN_vkGetRenderAreaGranularity GetRenderAreaGranularity = nullptr;
    PFN_vkInvalidateMappedMemoryRanges InvalidateMappedMemoryRanges = nullptr;
    PFN_vkMapMemory MapMemory = nullptr;
    PFN_vkMergePipelineCaches MergePipelineCaches = nullptr;
    PFN_vkQueueBindSparse QueueBindSparse = nullptr;
    PFN_vkQueueSubmit QueueSubmit = nullptr;
    PFN_vkQueueWaitIdle QueueWaitIdle = nullptr;
    PFN_vkResetCommandBuffer ResetCommandBuffer = nullptr;
    PFN_vkResetCommandPool ResetCommandPool = nullptr;
    PFN_vkResetDescriptorPool ResetDescriptorPool = nullptr;
    PFN_vkResetEvent ResetEvent = nullptr;
    PFN_vkResetFences ResetFences = nullptr;
    PFN_vkSetEvent SetEvent = nullptr;
    PFN_vkUnmapMemory UnmapMemory = nullptr;
    PFN_vkUpdateDescriptorSets UpdateDescriptorSets = nullptr;
    PFN_vkWaitForFences WaitForFences = nullptr;

    // VK_EXT_debug_marker
    PFN_vkCmdDebugMarkerBeginEXT CmdDebugMarkerBeginEXT = nullptr;
    PFN_vkCmdDebugMarkerEndEXT CmdDebugMarkerEndEXT = nullptr;
    PFN_vkCmdDebugMarkerInsertEXT CmdDebugMarkerInsertEXT = nullptr;

    // VK_KHR_swapchain
    PFN_vkCreateSwapchainKHR CreateSwapchainKHR = nullptr;
    PFN_vkDestroySwapchainKHR DestroySwapchainKHR = nullptr;
    PFN_vkGetSwapchainImagesKHR GetSwapchainImagesKHR = nullptr;
    PFN_vkAcquireNextImageKHR AcquireNextImageKHR = nullptr;
    PFN_vkQueuePresentKHR QueuePresentKHR = nullptr;

    // VK_KHR_external_memory_fd
    PFN_vkGetMemoryFdKHR GetMemoryFdKHR = nullptr;
    PFN_vkGetMemoryFdPropertiesKHR GetMemoryFdPropertiesKHR = nullptr;

    // VK_KHR_external_semaphore_fd
    PFN_vkImportSemaphoreFdKHR ImportSemaphoreFdKHR = nullptr;
    PFN_vkGetSemaphoreFdKHR GetSemaphoreFdKHR = nullptr;

#ifdef VK_USE_PLATFORM_FUCHSIA
    // VK_FUCHSIA_external_memory
    PFN_vkGetMemoryZirconHandleFUCHSIA GetMemoryZirconHandleFUCHSIA = nullptr;
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA GetMemoryZirconHandlePropertiesFUCHSIA =
        nullptr;

    // VK_FUCHSIA_external_semaphore
    PFN_vkImportSemaphoreZirconHandleFUCHSIA ImportSemaphoreZirconHandleFUCHSIA = nullptr;
    PFN_vkGetSemaphoreZirconHandleFUCHSIA GetSemaphoreZirconHandleFUCHSIA = nullptr;
#endif

#if defined(VK_USE_PLATFORM_MACOS_MVK)
    // VK_MVK_macos_surface
    PFN_vkCreateMacOSSurfaceMVK CreateMacOSSurfaceMVK = nullptr;
#endif

#if defined(VK_USE_PLATFORM_METAL_EXT)
    // VK_EXT_metal_surface
    PFN_vkCreateMetalSurfaceEXT CreateMetalSurfaceEXT = nullptr;
#endif

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    // VK_KHR_win32_surface
    PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceKHR = nullptr;
#endif
};

} // namespace vkt