#include "application.h"

#include "vkt/gpu/vulkan/vulkan_buffer.h"
#include "vkt/gpu/vulkan/vulkan_command_buffer.h"
#include "vkt/gpu/vulkan/vulkan_device.h"
#include "vkt/gpu/vulkan/vulkan_physical_device.h"
#include "vkt/gpu/vulkan/vulkan_pipeline.h"
#include "vkt/gpu/vulkan/vulkan_render_pass.h"
#include "vkt/gpu/vulkan/vulkan_swapchain.h"
#include "vkt/gpu/vulkan/vulkan_texture_view.h"

#include "utils/log.h"
#include "window.h"
#include <glm/glm.hpp>
#include <string>

std::filesystem::path Application::path;
std::filesystem::path Application::dir;

#ifdef NDEBUG
const bool enableValidationLayers = true;
const bool enableDebugMessenger = true;
#else
const bool enableValidationLayers = false;
const bool enableDebugMessenger = false;
#endif

const std::vector<const char*> requiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    if (messageType != 1)
    {
        LOG_ERROR("pCallbackData->pMessage : {}", pCallbackData->pMessage);
    }

    return VK_FALSE;
}

Application::Application(int argc, char** argv)
{
    path = std::filesystem::path(argv[0]);
    dir = path.parent_path();
}

void Application::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void Application::initWindow()
{
    m_window = new Window(800, 600, "Triangle Window");
}

void Application::initVulkan()
{
    // create Driver.
    {
        DriverDescriptor descriptor{ .type = DRIVER_TYPE::VULKAN };
        m_driver = Driver::create(descriptor);
    }

    // create surface
    {
        SurfaceDescriptor descriptor{ .windowHandle = m_window->getNativeWindow() };
        m_surface = m_driver->createSurface(descriptor);
    }

    // create PhysicalDevice.
    {
        PhysicalDeviceDescriptor descriptor{};
        m_physicalDevice = m_driver->createPhysicalDevice(descriptor);
    }

    // create Device.
    {
        DeviceDescriptor descriptor{};
        m_device = m_physicalDevice->createDevice(descriptor);
    }

    // create swapchain
    {
        SwapChainDescriptor swapChainCreateInfo{ .textureFormat = TextureFormat::kBGRA_8888_UInt_Norm,
                                                 .presentMode = PresentMode::kFifo,
                                                 .colorSpace = ColorSpace::kSRGBNonLinear,
                                                 .width = 800,
                                                 .height = 600,
                                                 .surface = m_surface.get() };
        m_swapChain = m_device->createSwapChain(swapChainCreateInfo);
    }

    // create buffer
    {
        m_vertices = {
            { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
            { { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
            { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
        };

        uint64_t size = static_cast<uint64_t>(sizeof(Vertex) * m_vertices.size());
        BufferDescriptor bufferDescriptor{ .size = size,
                                           .usage = BufferUsage::kVertex };
        m_buffer = m_device->createBuffer(bufferDescriptor);

        void* mappedPointer = m_buffer->map();
        memcpy(mappedPointer, m_vertices.data(), size);
        m_buffer->unmap();
    }

    createGraphicsPipeline();
    createCommandBuffers();
    createSemaphores();
}

void Application::mainLoop()
{
    while (!m_window->shouldClose())
    {
        drawFrame();
    }

    // vkDeviceWaitIdle(m_context.device);
}

void Application::cleanup()
{
    // vkDestroySemaphore(m_context.device, m_renderFinishedSemaphore, nullptr);
    // vkDestroySemaphore(m_context.device, m_imageAvailableSemaphore, nullptr);

    // vkDestroyCommandPool(m_context.device, m_commandPool, nullptr);

    // for (auto framebuffer : m_vecSwapChainFramebuffers)
    // {
    //     vkDestroyFramebuffer(m_context.device, framebuffer, nullptr);
    // }

    //    m_pipeline.destroy();

    //    vkDestroyRenderPass(m_context.device, m_renderPass, nullptr);

    //    for (const VkImageView& imageView : m_swapChain->getImageViews()) // TODO: move into SwapChain object.
    //    {
    //        vkDestroyImageView(m_context.device, imageView, nullptr);
    //    }

    //    vkDestroySwapchainKHR(m_context.device, static_cast<VkSwapchainKHR>(m_swapChain->getHandle()), nullptr); //
    //    TODO: move into SwapChain object.

    //    vkDestroyDevice(m_context.device, nullptr);

    // DestroyDebugUtilsMessengerEXT(m_context.instance, m_debugMessenger, nullptr);

    //    vkDestroySurfaceKHR(m_context.instance, m_context.surface, nullptr);

    //    m_context.finalize(); // vkDestroyInstance(m_context.instance, nullptr);

    delete m_window; // glfwDestroyWindow(m_window);

    glfwTerminate();
}

void Application::createGraphicsPipeline()
{
    // create pipeline
    {
        PipelineDescriptor descriptor{};
        m_pipeline = m_device->createPipeline(descriptor);
    }

    auto vulkanPipeline = downcast(m_pipeline.get());
    auto vulkanDevice = downcast(m_device.get());
    VulkanRenderPassDescriptor renderPassDescriptor{ .format = VK_FORMAT_B8G8R8A8_SRGB,
                                                     .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                     .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                                                     .samples = VK_SAMPLE_COUNT_1_BIT };
    auto vulkanRenderPass = vulkanDevice->getRenderPass(renderPassDescriptor);
    vulkanPipeline->setRenderPass(vulkanRenderPass);

    vulkanPipeline->createGraphicsPipeline((Application::getDir() / "triangle_vert.spv").generic_string(),
                                           (Application::getDir() / "triangle_frag.spv").generic_string());
}

void Application::createCommandBuffers()
{

    std::vector<TextureView*> swapChainTextureViews = m_swapChain->getTextureViews();

    auto commandBufferCount = swapChainTextureViews.size();
    m_commandBuffers.resize(commandBufferCount);
    for (auto i = 0; i < commandBufferCount; ++i)
    {
        CommandBufferDescriptor descriptor{};
        auto commandBuffer = m_device->createCommandBuffer(descriptor);
        m_commandBuffers[i] = std::move(commandBuffer);
    }

    for (auto i = 0; i < commandBufferCount; ++i)
    {
        auto commandBuffer = m_commandBuffers[i].get();

        std::vector<ColorAttachment> colorAttachments(1); // in currently. use only one.
        colorAttachments[0] = { .textureView = swapChainTextureViews[i],
                                .loadOp = LoadOp::kClear,
                                .storeOp = StoreOp::kStore,
                                .clearValue = { .float32 = { 0.0f, 0.0f, 0.0f, 1.0f } } };
        DepthStencilAttachment depthStencilAttachment{};

        CommandEncoderDescriptor descriptor{ .colorAttachments = colorAttachments,
                                             .depthStencilAttachment = depthStencilAttachment };
        auto commandEncoder = commandBuffer->createCommandEncoder(descriptor);
        commandEncoder->begin();
        commandEncoder->setPipeline(m_pipeline.get());
        commandEncoder->setVertexBuffer(m_buffer.get());
        commandEncoder->draw(static_cast<uint32_t>(m_vertices.size()));
        commandEncoder->end();
    }
}

void Application::drawFrame()
{
    uint32_t imageIndex;
    auto vulkanSwapChain = downcast(m_swapChain.get());
    VkSwapchainKHR swapChain = vulkanSwapChain->getVkSwapchainKHR();

    VulkanDevice* vulkanDevice = downcast(m_device.get());
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;
    vkAPI.AcquireNextImageKHR(vulkanDevice->getVkDevice(), swapChain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
    VkPipelineStageFlags waitPipelineStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitPipelineStages;

    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer = downcast(m_commandBuffers[imageIndex].get())->getVkCommandBuffer();
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    VkQueue queue = vulkanDevice->getQueue();
    if (vkAPI.QueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        LOG_ERROR("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    vkAPI.QueuePresentKHR(queue, &presentInfo);

    vkAPI.QueueWaitIdle(vulkanDevice->getQueue());
}

void Application::createSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VulkanDevice* vulkanDevice = downcast(m_device.get());
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;
    if (vkAPI.CreateSemaphore(vulkanDevice->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
        vkAPI.CreateSemaphore(vulkanDevice->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS)
    {
        LOG_ERROR("failed to create semaphores!");
    }
}
