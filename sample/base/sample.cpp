#include "sample.h"

#include <algorithm>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

#include "hpc/counter.h"
#include "hpc/hpc.h"

namespace jipu
{

Sample::Sample(const SampleDescriptor& descriptor)
    : Window(descriptor.windowDescriptor)
    , m_appPath(descriptor.path)
    , m_appDir(descriptor.path.parent_path())
    , m_imgui(Im_Gui())
{
}

Sample::~Sample()
{
    if (m_imgui.has_value())
    {
        m_imgui.value().clear();
    }

    m_queue.reset();
    m_swapchain.reset();
    m_surface.reset();
    m_device.reset();
    m_physicalDevices.clear();
    m_driver.reset();
}

void Sample::init()
{
    createDriver();
    getPhysicalDevices();
    createSurface();
    createDevice();
    createSwapchain();
    createQueue();

    if (m_imgui.has_value())
    {
        m_imgui.value().init(m_device.get(), m_queue.get(), *m_swapchain);
    }

    Window::init();
}

void Sample::update()
{
    if (m_hpcWatcher)
    {
        m_hpcWatcher->update();
    }
}

void Sample::recordImGui(std::vector<std::function<void()>> cmds)
{
    if (m_imgui.has_value())
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

        m_imgui.value().record(cmds);
        m_imgui.value().build();
    }
}

void Sample::windowImGui(const char* title, std::vector<std::function<void()>> uis)
{
    if (m_imgui.has_value())
    {
        m_imgui.value().window(title, uis);
    }
}

void Sample::drawImGui(CommandEncoder* commandEncoder, TextureView& renderView)
{
    if (m_imgui.has_value())
    {
        m_fps.update();
        m_imgui.value().draw(commandEncoder, renderView);
    }
}

void Sample::onHPCListner(std::unordered_map<Counter, double> values)
{
    for (const auto& [k, v] : values)
    {
        spdlog::debug("{}: {}", static_cast<uint32_t>(k), v);
    }
}

void Sample::createDriver()
{
    DriverDescriptor descriptor;
    descriptor.type = DriverType::kVulkan;
    m_driver = Driver::create(descriptor);
}

void Sample::getPhysicalDevices()
{
    m_physicalDevices = m_driver->getPhysicalDevices();
}

void Sample::createSurface()
{
    SurfaceDescriptor descriptor;
    descriptor.windowHandle = getWindowHandle();
    m_surface = m_driver->createSurface(descriptor);
}

void Sample::createSwapchain()
{
    if (m_surface == nullptr)
        throw std::runtime_error("Surface is null pointer.");

#if defined(__ANDROID__) || defined(ANDROID)
    TextureFormat textureFormat = TextureFormat::kRGBA_8888_UInt_Norm_SRGB;
#else
    TextureFormat textureFormat = TextureFormat::kBGRA_8888_UInt_Norm_SRGB;
#endif
    SwapchainDescriptor descriptor{
        .surface = *m_surface,
        .textureFormat = textureFormat,
        .presentMode = PresentMode::kFifo,
        .colorSpace = ColorSpace::kSRGBNonLinear,
        .width = m_width,
        .height = m_height
    };

    m_swapchain = m_device->createSwapchain(descriptor);
}

void Sample::createDevice()
{
    // TODO: select suit device.
    PhysicalDevice* physicalDevice = m_physicalDevices[0].get();

    DeviceDescriptor descriptor;
    m_device = physicalDevice->createDevice(descriptor);
}

void Sample::createQueue()
{
    QueueDescriptor descriptor{};
    descriptor.flags = QueueFlagBits::kGraphics;

    m_queue = m_device->createQueue(descriptor);
}

void Sample::createHPCWatcher(std::vector<Counter> counters)
{
    auto gpus = hpc::gpus();
    if (!gpus.empty())
    {
        // TODO: select gpu.
        auto gpu = gpus[0].get();
        const auto& hpcCounters = gpu->counters();
        hpc::SamplerDescriptor descriptor{ .counters = hpcCounters };
        hpc::Sampler::Ptr sampler = gpu->create(descriptor);

        HPCWatcherDescriptor watcherDescriptor{
            .sampler = std::move(sampler),
            .counters = counters,
            .listner = std::bind(&Sample::onHPCListner, this, std::placeholders::_1)
        };
        m_hpcWatcher = std::make_unique<HPCWatcher>(std::move(watcherDescriptor));
        m_hpcWatcher->start();
    }
}

void Sample::debuggingWindow()
{
    windowImGui(
        "Debugging", { [&]() {
            const auto& fpss = m_fps.getAll();
            if (!fpss.empty())
            {
                const auto size = fpss.size();
                const std::string title = fmt::format("FPS", m_fps.average());
                const std::string description = fmt::format("{:.1f}", m_fps.current());
                int offset = 0;
                if (size > 15)
                    offset = size - 15;
                ImGui::PlotLines(title.c_str(), &fpss[offset], size - offset, 0, description.c_str());
            }

            if (m_hpcWatcher)
                ImGui::Checkbox("Profiler", &m_profiling);
        } });

    if (m_profiling)
    {
        // show profiling.
    }
}

} // namespace jipu