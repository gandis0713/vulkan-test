#include "vulkan_n_buffering_sample.h"

#include <math.h>
#include <spdlog/spdlog.h>

#include <random>
#include <stdexcept>

#include "vulkan_device.h"
#include "vulkan_swapchain.h"

namespace jipu
{
VulkanNBufferingSample::VulkanNBufferingSample(const SampleDescriptor& descriptor)
    : Sample(descriptor)
{
}

VulkanNBufferingSample::~VulkanNBufferingSample()
{
    clearImGui();

    m_vertexShaderModule.reset();
    m_fragmentShaderModule.reset();

    m_renderPipeline.reset();
    m_pipelineLayout.reset();

    m_bindingGroupLayouts.clear();
    m_bindingGroups.clear();

    m_depthStencilTextureView.reset();
    m_depthStencilTexture.reset();
    m_colorAttachmentTextureView.reset();
    m_colorAttachmentTexture.reset();

    // unmap m_uniformBufferMappedPointer;
    m_uniformBuffer.reset();

    m_imageSampler.reset();
    m_imageTextureView.reset();
    m_imageTexture.reset();

    m_indexBuffer.reset();
    m_vertexBuffer.reset();

    m_queue.reset();

    // release command buffer after finising queue.
    m_renderCommandBuffer.reset();
    m_swapchain.reset();

    m_physicalDevices.clear();
    m_device.reset();

    m_surface.reset();
    m_driver.reset();
}

void VulkanNBufferingSample::init()
{
    // create Driver.
    {
        DriverDescriptor descriptor{ .type = DriverType::kVulkan };
        m_driver = Driver::create(descriptor);
    }

    // create surface
    {
        SurfaceDescriptor descriptor{ .windowHandle = getWindowHandle() };
        m_surface = m_driver->createSurface(descriptor);
    }

    // create PhysicalDevice.
    {
        m_physicalDevices = m_driver->getPhysicalDevices();
    }

    // create Device.
    {
        // TODO: select suit device.
        PhysicalDevice* physicalDevice = m_physicalDevices[0].get();

        DeviceDescriptor descriptor;
        m_device = physicalDevice->createDevice(descriptor);
    }

    // create queue
    {
        QueueDescriptor rednerQueueDescriptor{ .flags = QueueFlagBits::kGraphics | QueueFlagBits::kTransfer };
        m_queue = m_device->createQueue(rednerQueueDescriptor);
    }

    // create swapchain
    createSwapchain();

    // create buffer
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffer();

    createImageTexture();
    createImageTextureView();
    createImageSampler();

    createColorAttachmentTexture();
    createColorAttachmentTextureView();
    createDepthStencilTexture();
    createDepthStencilTextureView();

    createBindingGroupLayout();
    createBindingGroup();

    createPipelineLayout();
    createRenderPipeline();
    createCommandBuffers();

    initImGui(m_device.get(), m_queue.get(), m_swapchain.get());

    m_initialized = true;
}

void VulkanNBufferingSample::update()
{
    updateUniformBuffer();

    updateImGui();
    buildImGui();
}

void VulkanNBufferingSample::updateImGui()
{
    // set display size and mouse state.
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_width, (float)m_height);
        io.MousePos = ImVec2(m_mouseX, m_mouseY);
        io.MouseDown[0] = m_leftMouseButton;
        io.MouseDown[1] = m_rightMouseButton;
        io.MouseDown[2] = m_middleMouseButton;
    }

    ImGui::NewFrame();

    // set windows position and size
    {
        auto scale = ImGui::GetIO().FontGlobalScale;
        ImGui::SetNextWindowPos(ImVec2(20, 20 + m_padding.top), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300 * scale, 100 * scale), ImGuiCond_FirstUseEver);
    }

    // set ui
    {
        ImGui::Begin("Settings");
        if (ImGui::RadioButton("FIFO", m_presentMode == VK_PRESENT_MODE_FIFO_KHR))
        {
            m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
            recreateSwapchain();
        }
        else if (ImGui::RadioButton("FIFO RELAXED", m_presentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR))
        {
            m_presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
            recreateSwapchain();
        }
        else if (ImGui::RadioButton("MAILBOX", m_presentMode == VK_PRESENT_MODE_MAILBOX_KHR))
        {
            m_presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            recreateSwapchain();
        }

        ImGui::Separator();

        if (ImGui::RadioButton("Min Count 2", m_minImageCount == 2))
        {
            m_minImageCount = 2;
            recreateSwapchain();
        }
        else if (ImGui::RadioButton("Min Count 3", m_minImageCount == 3))
        {
            m_minImageCount = 3;
            recreateSwapchain();
        }
        ImGui::End();
    }

    debugWindow();
    ImGui::Render();
}

void VulkanNBufferingSample::draw()
{
    auto renderView = m_swapchain->acquireNextTexture();

    CommandEncoderDescriptor commandEncoderDescriptor{};
    std::unique_ptr<CommandEncoder> commandEncoder = m_renderCommandBuffer->createCommandEncoder(commandEncoderDescriptor);

    std::vector<ColorAttachment> colorAttachments(1); // in currently. use only one.
    colorAttachments[0] = { .renderView = m_sampleCount > 1 ? m_colorAttachmentTextureView.get() : renderView,
                            .resolveView = m_sampleCount > 1 ? renderView : nullptr,
                            .loadOp = LoadOp::kClear,
                            .storeOp = StoreOp::kStore,
                            .clearValue = { .float32 = { 0.0f, 0.0f, 0.0f, 1.0f } } };
    DepthStencilAttachment depthStencilAttachment{ .textureView = m_depthStencilTextureView.get(),
                                                   .depthLoadOp = LoadOp::kClear,
                                                   .depthStoreOp = StoreOp::kStore,
                                                   .stencilLoadOp = LoadOp::kDontCare,
                                                   .stencilStoreOp = StoreOp::kDontCare,
                                                   .clearValue = { .depth = 1.0f, .stencil = 0 } };

    RenderPassEncoderDescriptor renderPassDescriptor{ .colorAttachments = colorAttachments,
                                                      .depthStencilAttachment = depthStencilAttachment,
                                                      .sampleCount = m_sampleCount };

    std::unique_ptr<RenderPassEncoder> renderPassEncoder = commandEncoder->beginRenderPass(renderPassDescriptor);
    renderPassEncoder->setPipeline(m_renderPipeline.get());
    renderPassEncoder->setBindingGroup(0, m_bindingGroups[0].get());
    renderPassEncoder->setBindingGroup(1, m_bindingGroups[1].get());
    renderPassEncoder->setVertexBuffer(0, m_vertexBuffer.get());
    renderPassEncoder->setIndexBuffer(m_indexBuffer.get(), IndexFormat::kUint16);
    renderPassEncoder->setViewport(0, 0, m_width, m_height, 0, 1); // set viewport state.
    renderPassEncoder->setScissor(0, 0, m_width, m_height);        // set scissor state.
    renderPassEncoder->drawIndexed(static_cast<uint32_t>(m_polygon.indices.size()), 1, 0, 0, 0);
    renderPassEncoder->end();

    drawImGui(commandEncoder.get(), renderView);

    m_queue->submit({ commandEncoder->finish() }, m_swapchain.get());
}

void VulkanNBufferingSample::createSwapchain()
{
    if (m_surface == nullptr)
        throw std::runtime_error("Surface is null pointer.");

#if defined(__ANDROID__) || defined(ANDROID)
    TextureFormat textureFormat = TextureFormat::kRGBA_8888_UInt_Norm_SRGB;
#else
    TextureFormat textureFormat = TextureFormat::kBGRA_8888_UInt_Norm_SRGB;
#endif

    SwapchainDescriptor descriptor;
    descriptor.width = m_width;
    descriptor.height = m_height;
    descriptor.surface = m_surface.get();
    descriptor.colorSpace = ColorSpace::kSRGBNonLinear;
    descriptor.textureFormat = textureFormat;
    descriptor.presentMode = PresentMode::kFifo;

    auto vulkanDevice = downcast(m_device.get());
    VulkanSwapchainDescriptor vkdescriptor = generateVulkanSwapchainDescriptor(vulkanDevice, descriptor);

    vkdescriptor.presentMode = m_presentMode;
    vkdescriptor.minImageCount = m_minImageCount;
    vkdescriptor.oldSwapchain = m_swapchain != nullptr ? downcast(m_swapchain.get())->getVkSwapchainKHR() : VK_NULL_HANDLE;

    auto swapchain = vulkanDevice->createSwapchain(vkdescriptor);

    m_swapchain = std::move(swapchain);
}

void VulkanNBufferingSample::createVertexBuffer()
{
    // load obj as buffer for android.
    std::vector<char> buffer = utils::readFile(m_appDir / "spaceship.obj", m_handle);
    m_polygon = loadOBJ(buffer.data(), buffer.size());

    uint64_t vertexSize = static_cast<uint64_t>(sizeof(Vertex) * m_polygon.vertices.size());
    BufferDescriptor vertexStagingBufferDescriptor{ .size = vertexSize,
                                                    .usage = BufferUsageFlagBits::kCopySrc };
    std::unique_ptr<Buffer> vertexStagingBuffer = m_device->createBuffer(vertexStagingBufferDescriptor);

    void* mappedPointer = vertexStagingBuffer->map();
    memcpy(mappedPointer, m_polygon.vertices.data(), vertexSize);

    BufferDescriptor vertexBufferDescriptor{ .size = vertexSize,
                                             .usage = BufferUsageFlagBits::kVertex | BufferUsageFlagBits::kCopyDst };
    m_vertexBuffer = m_device->createBuffer(vertexBufferDescriptor);

    // copy staging buffer to target buffer
    copyBufferToBuffer(vertexStagingBuffer.get(), m_vertexBuffer.get());

    // unmap staging buffer.
    // vertexStagingBuffer->unmap(); // TODO: need unmap before destroy it?
}

void VulkanNBufferingSample::createIndexBuffer()
{
    uint64_t indexSize = static_cast<uint64_t>(sizeof(uint16_t) * m_polygon.indices.size());
    BufferDescriptor indexBufferDescriptor{ .size = indexSize,
                                            .usage = BufferUsageFlagBits::kIndex };

    m_indexBuffer = m_device->createBuffer(indexBufferDescriptor);

    void* mappedPointer = m_indexBuffer->map();
    memcpy(mappedPointer, m_polygon.indices.data(), indexSize);
    m_indexBuffer->unmap();
}

void VulkanNBufferingSample::createUniformBuffer()
{
    BufferDescriptor descriptor{ .size = sizeof(UniformBufferObject),
                                 .usage = BufferUsageFlagBits::kUniform };
    m_uniformBuffer = m_device->createBuffer(descriptor);
    m_uniformBufferMappedPointer = m_uniformBuffer->map();
}

void VulkanNBufferingSample::createImageTexture()
{
    // load as buffer for android.
    std::vector<char> buffer = utils::readFile(m_appDir / "spaceship_color_4.jpg", m_handle);
    m_image = std::make_unique<Image>(buffer.data(), buffer.size());

    unsigned char* pixels = static_cast<unsigned char*>(m_image->getPixels());
    uint32_t width = m_image->getWidth();
    uint32_t height = m_image->getHeight();
    uint32_t channel = m_image->getChannel();
    uint64_t imageSize = sizeof(unsigned char) * width * height * channel;

    uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

    // create image staging buffer.
    BufferDescriptor descriptor{ .size = imageSize, .usage = BufferUsageFlagBits::kCopySrc };
    std::unique_ptr<Buffer> imageTextureStagingBuffer = m_device->createBuffer(descriptor);

    void* mappedPointer = imageTextureStagingBuffer->map();
    memcpy(mappedPointer, pixels, imageSize);

    // create texture.
    TextureDescriptor textureDescriptor{ .type = TextureType::k2D,
                                         .format = TextureFormat::kRGBA_8888_UInt_Norm_SRGB,
                                         .usage = TextureUsageFlagBits::kCopySrc |
                                                  TextureUsageFlagBits::kCopyDst |
                                                  TextureUsageFlagBits::kTextureBinding,
                                         .width = width,
                                         .height = height,
                                         .depth = 1,
                                         .mipLevels = mipLevels,
                                         .sampleCount = 1 };
    m_imageTexture = m_device->createTexture(textureDescriptor);

    // copy image staging buffer to texture
    copyBufferToTexture(imageTextureStagingBuffer.get(), m_imageTexture.get());

    // unmap staging buffer
    // imageTextureStagingBuffer->unmap(); // TODO: need unmap before destroy it?
}

void VulkanNBufferingSample::createImageTextureView()
{
    TextureViewDescriptor descriptor{};
    descriptor.type = TextureViewType::k2D;
    descriptor.aspect = TextureAspectFlagBits::kColor;

    m_imageTextureView = m_imageTexture->createTextureView(descriptor);
}

void VulkanNBufferingSample::createColorAttachmentTexture()
{
    // create color texture.
    TextureDescriptor textureDescriptor{ .type = TextureType::k2D,
                                         .format = m_swapchain->getTextureFormat(),
                                         .usage = TextureUsageFlagBits::kColorAttachment,
                                         .width = m_swapchain->getWidth(),
                                         .height = m_swapchain->getHeight(),
                                         .depth = 1,
                                         .mipLevels = 1,
                                         .sampleCount = m_sampleCount };
    m_colorAttachmentTexture = m_device->createTexture(textureDescriptor);
}

void VulkanNBufferingSample::createColorAttachmentTextureView()
{
    TextureViewDescriptor descriptor{};
    descriptor.type = TextureViewType::k2D;
    descriptor.aspect = TextureAspectFlagBits::kColor;

    m_colorAttachmentTextureView = m_colorAttachmentTexture->createTextureView(descriptor);
}

void VulkanNBufferingSample::createDepthStencilTexture()
{
    TextureDescriptor descriptor{};
    descriptor.type = TextureType::k2D;
    descriptor.format = TextureFormat::kD_32_SFloat;
    descriptor.usage = TextureUsageFlagBits::kDepthStencil;
    descriptor.mipLevels = 1;
    descriptor.width = m_swapchain->getWidth();
    descriptor.height = m_swapchain->getHeight();
    descriptor.depth = 1;
    descriptor.sampleCount = m_sampleCount;

    m_depthStencilTexture = m_device->createTexture(descriptor);
}

void VulkanNBufferingSample::createDepthStencilTextureView()
{
    TextureViewDescriptor descriptor{};
    descriptor.type = TextureViewType::k2D;
    descriptor.aspect = TextureAspectFlagBits::kDepth;

    m_depthStencilTextureView = m_depthStencilTexture->createTextureView(descriptor);
}

void VulkanNBufferingSample::createImageSampler()
{
    SamplerDescriptor descriptor{};
    descriptor.magFilter = FilterMode::kLinear;
    descriptor.minFilter = FilterMode::kLinear;
    descriptor.mipmapFilter = MipmapFilterMode::kLinear;
    descriptor.addressModeU = AddressMode::kClampToEdge;
    descriptor.addressModeV = AddressMode::kClampToEdge;
    descriptor.addressModeW = AddressMode::kClampToEdge;
    descriptor.lodMin = 0.0f;
    // descriptor.lodMin = static_cast<float>(m_imageTexture->getMipLevels() / 2);
    descriptor.lodMax = static_cast<float>(m_imageTexture->getMipLevels());

    m_imageSampler = m_device->createSampler(descriptor);
}

void VulkanNBufferingSample::createBindingGroupLayout()
{
    m_bindingGroupLayouts.resize(2);
    {
        // Uniform Buffer
        BufferBindingLayout bufferBindingLayout{};
        bufferBindingLayout.type = BufferBindingType::kUniform;
        bufferBindingLayout.index = 0;
        bufferBindingLayout.stages = BindingStageFlagBits::kVertexStage;
        std::vector<BufferBindingLayout> bufferBindingLayouts{ bufferBindingLayout };

        BindingGroupLayoutDescriptor bindingGroupLayoutDescriptor{ .buffers = bufferBindingLayouts };

        m_bindingGroupLayouts[0] = m_device->createBindingGroupLayout(bindingGroupLayoutDescriptor);
    }
    {
        // Sampler
        SamplerBindingLayout samplerBindingLayout{};
        samplerBindingLayout.index = 0;
        samplerBindingLayout.stages = BindingStageFlagBits::kFragmentStage;

        std::vector<SamplerBindingLayout> samplerBindingLayouts{ samplerBindingLayout };

        // Texture
        TextureBindingLayout textureBindingLayout{};
        textureBindingLayout.index = 1;
        textureBindingLayout.stages = BindingStageFlagBits::kFragmentStage;

        std::vector<TextureBindingLayout> textureBindingLayouts{ textureBindingLayout };

        BindingGroupLayoutDescriptor bindingGroupLayoutDescriptor{ .buffers = {},
                                                                   .samplers = samplerBindingLayouts,
                                                                   .textures = textureBindingLayouts };

        m_bindingGroupLayouts[1] = m_device->createBindingGroupLayout(bindingGroupLayoutDescriptor);
    }
}

void VulkanNBufferingSample::createBindingGroup()
{
    m_bindingGroups.resize(2);
    {
        BufferBinding bufferBinding{};
        bufferBinding.index = 0;
        bufferBinding.buffer = m_uniformBuffer.get();
        bufferBinding.offset = 0;
        bufferBinding.size = sizeof(UniformBufferObject);

        BindingGroupDescriptor descriptor{};
        descriptor.layout = m_bindingGroupLayouts[0].get();
        descriptor.buffers = { bufferBinding };

        m_bindingGroups[0] = m_device->createBindingGroup(descriptor);
    }

    {
        SamplerBinding samplerBinding{};
        samplerBinding.index = 0;
        samplerBinding.sampler = m_imageSampler.get();

        TextureBinding textureBinding{};
        textureBinding.index = 1;
        textureBinding.textureView = m_imageTextureView.get();

        BindingGroupDescriptor descriptor{};
        descriptor.layout = m_bindingGroupLayouts[1].get();
        descriptor.samplers = { samplerBinding };
        descriptor.textures = { textureBinding };

        m_bindingGroups[1] = m_device->createBindingGroup(descriptor);
    }
}

void VulkanNBufferingSample::createPipelineLayout()
{
    PipelineLayoutDescriptor pipelineLayoutDescriptor{ .layouts = { m_bindingGroupLayouts[0].get(), m_bindingGroupLayouts[1].get() } };
    m_pipelineLayout = m_device->createPipelineLayout(pipelineLayoutDescriptor);
}

void VulkanNBufferingSample::createRenderPipeline()
{

    // Input Assembly
    InputAssemblyStage inputAssembly{};
    {
        inputAssembly.topology = PrimitiveTopology::kTriangleList;
    }

    // vertex stage
    VertexStage vertexStage{};
    {
        // create vertex shader
        const std::vector<char> vertShaderCode = utils::readFile(m_appDir / "vulkan_n_buffering.vert.spv", m_handle);
        ShaderModuleDescriptor vertexShaderModuleDescriptor{ .code = vertShaderCode.data(),
                                                             .codeSize = vertShaderCode.size() };
        m_vertexShaderModule = m_device->createShaderModule(vertexShaderModuleDescriptor);
        vertexStage.shaderModule = m_vertexShaderModule.get();

        // layouts
        std::vector<VertexInputLayout> layouts{};
        layouts.resize(1);
        {
            // attributes
            std::vector<VertexAttribute> vertexAttributes{};
            vertexAttributes.resize(2);
            {
                // position
                vertexAttributes[0] = { .format = VertexFormat::kSFLOATx3,
                                        .offset = offsetof(Vertex, pos),
                                        .location = 0 };

                // texture coodinate
                vertexAttributes[1] = { .format = VertexFormat::kSFLOATx2,
                                        .offset = offsetof(Vertex, texCoord),
                                        .location = 1 };
            }

            VertexInputLayout vertexLayout{ .mode = VertexMode::kVertex,
                                            .stride = sizeof(Vertex),
                                            .attributes = vertexAttributes };
            layouts[0] = vertexLayout;
        }

        vertexStage.layouts = layouts;
    }

    // Rasterization
    RasterizationStage rasterization{};
    {
        rasterization.sampleCount = m_sampleCount;
        rasterization.cullMode = CullMode::kNone;
        rasterization.frontFace = FrontFace::kCounterClockwise;
    }

    // fragment stage
    FragmentStage fragmentStage{};
    {
        // create fragment shader
        const std::vector<char> fragShaderCode = utils::readFile(m_appDir / "vulkan_n_buffering.frag.spv", m_handle);
        ShaderModuleDescriptor fragmentShaderModuleDescriptor{ .code = fragShaderCode.data(),
                                                               .codeSize = fragShaderCode.size() };
        m_fragmentShaderModule = m_device->createShaderModule(fragmentShaderModuleDescriptor);

        fragmentStage.shaderModule = m_fragmentShaderModule.get();

        // output targets
        fragmentStage.targets = { { .format = m_swapchain->getTextureFormat() } };
    }

    // Depth/Stencil stage
    DepthStencilStage depthStencilStage;
    {
        depthStencilStage.format = m_depthStencilTexture->getFormat();
    }

    RenderPipelineDescriptor descriptor;
    descriptor.layout = m_pipelineLayout.get();
    descriptor.inputAssembly = inputAssembly;
    descriptor.vertex = vertexStage;
    descriptor.rasterization = rasterization;
    descriptor.fragment = fragmentStage;
    descriptor.depthStencil = depthStencilStage;

    m_renderPipeline = m_device->createRenderPipeline(descriptor);
}

void VulkanNBufferingSample::createCommandBuffers()
{
    CommandBufferDescriptor descriptor{ .usage = CommandBufferUsage::kOneTime };
    m_renderCommandBuffer = m_device->createCommandBuffer(descriptor);
}

void VulkanNBufferingSample::copyBufferToBuffer(Buffer* src, Buffer* dst)
{
    BlitBuffer srcBuffer{};
    srcBuffer.buffer = src;
    srcBuffer.offset = 0;

    BlitBuffer dstBuffer{};
    dstBuffer.buffer = dst;
    dstBuffer.offset = 0;

    CommandBufferDescriptor commandBufferDescriptor{ .usage = CommandBufferUsage::kOneTime };
    auto commandBuffer = m_device->createCommandBuffer(commandBufferDescriptor);

    CommandEncoderDescriptor commandEncoderDescriptor{};
    auto commandEncoder = commandBuffer->createCommandEncoder(commandEncoderDescriptor);

    commandEncoder->copyBufferToBuffer(srcBuffer, dstBuffer, src->getSize());

    m_queue->submit({ commandEncoder->finish() });
}

void VulkanNBufferingSample::copyBufferToTexture(Buffer* imageTextureStagingBuffer, Texture* imageTexture)
{
    BlitTextureBuffer blitTextureBuffer{};
    blitTextureBuffer.buffer = imageTextureStagingBuffer;
    blitTextureBuffer.offset = 0;
    uint32_t channel = 4;                          // TODO: from texture.
    uint32_t bytesPerData = sizeof(unsigned char); // TODO: from buffer.
    blitTextureBuffer.bytesPerRow = bytesPerData * imageTexture->getWidth() * channel;
    blitTextureBuffer.rowsPerTexture = imageTexture->getHeight();

    BlitTexture blitTexture{ .texture = imageTexture, .aspect = TextureAspectFlagBits::kColor };
    Extent3D extent{};
    extent.width = imageTexture->getWidth();
    extent.height = imageTexture->getHeight();
    extent.depth = 1;

    CommandBufferDescriptor commandBufferDescriptor{ .usage = CommandBufferUsage::kOneTime };
    std::unique_ptr<CommandBuffer> commandBuffer = m_device->createCommandBuffer(commandBufferDescriptor);

    CommandEncoderDescriptor commandEncoderDescriptor{};
    std::unique_ptr<CommandEncoder> commandEndoer = commandBuffer->createCommandEncoder(commandEncoderDescriptor);
    commandEndoer->copyBufferToTexture(blitTextureBuffer, blitTexture, extent);

    m_queue->submit({ commandEndoer->finish() });
}

void VulkanNBufferingSample::updateUniformBuffer()
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_swapchain->getWidth() / static_cast<float>(m_swapchain->getHeight()), 0.1f, 1000.0f);

    ubo.proj[1][1] *= -1;

    memcpy(m_uniformBufferMappedPointer, &ubo, sizeof(ubo));
}

void VulkanNBufferingSample::recreateSwapchain()
{
    auto vulkanDevice = downcast(m_device.get());
    vulkanDevice->vkAPI.DeviceWaitIdle(vulkanDevice->getVkDevice());
    createSwapchain();
}

} // namespace jipu