#pragma once

#include "jipu/common/ref_counted.h"
#include "jipu/native/adapter.h"
#include "jipu/native/surface.h"
#include "jipu/native/swapchain.h"
#include "jipu/webgpu/webgpu_header.h"

namespace jipu
{

class WebGPUDevice;
class WebGPUInstance;
class WebGPUAdapter;
class WebGPUSurface : public RefCounted
{

public:
    static WebGPUSurface* create(WebGPUInstance* instance, WGPUSurfaceDescriptor const* descriptor);

public:
    WebGPUSurface() = delete;
    explicit WebGPUSurface(WebGPUInstance* instance, WGPUSurfaceDescriptor const* descriptor);

public:
    ~WebGPUSurface() override = default;

    WebGPUSurface(const WebGPUSurface&) = delete;
    WebGPUSurface& operator=(const WebGPUSurface&) = delete;

public: // WebGPU API
    WGPUStatus getCapabilities(WebGPUAdapter* adapter, WGPUSurfaceCapabilities* capabilities);
    void configure(WGPUSurfaceConfiguration const* config);
    void getCurrentTexture(WGPUSurfaceTexture* surfaceTexture);
    void present();

public:
    enum class Type
    {
        kUndefined,
        kMetalLayer,
        kWindowsHWND,
        kAndroidWindow,
    };

private:
    [[maybe_unused]] WebGPUInstance* m_wgpuInstance = nullptr;
    [[maybe_unused]] const WGPUSurfaceDescriptor m_descriptor{};
    [[maybe_unused]] WGPUSurfaceConfiguration m_configuration{};

private:
    std::shared_ptr<Adapter> m_adapter = nullptr; // shared with adapter.
    std::unique_ptr<Surface> m_surface = nullptr;
    Swapchain* m_swapchain = nullptr;

private:
    Type m_type = Type::kUndefined;

    // metal
    void* m_meterLayer = nullptr;

    // android
    void* m_androidNativeWindow = nullptr;

    // windows
    void* m_hInstance = nullptr;
    void* m_HWND = nullptr;
};

// Convert from JIPU to WebGPU
WGPUCompositeAlphaMode ToWGPUCompositeAlphaMode(CompositeAlphaFlag flag);
WGPUPresentMode ToWGPUPresentMode(PresentMode mode);

// Convert from WebGPU to JIPU
CompositeAlphaFlag WPGUToCompositeAlphaMode(WGPUCompositeAlphaMode flag);
PresentMode WGPUToPresentMode(WGPUPresentMode mode);

} // namespace jipu