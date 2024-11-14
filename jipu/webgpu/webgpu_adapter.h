#pragma once

#include "jipu/common/ref_counted.h"
#include "jipu/native/adapter.h"
#include "jipu/native/physical_device.h"
#include "jipu/webgpu/webgpu_header.h"

namespace jipu
{

class WebGPUInstance;
class WebGPUAdapter : public RefCounted
{
public:
    static WebGPUAdapter* create(WebGPUInstance* wgpuInstance, WGPU_NULLABLE WGPURequestAdapterOptions const* options);

public:
    WebGPUAdapter() = delete;
    explicit WebGPUAdapter(WebGPUInstance* wgpuInstance,
                           std::unique_ptr<Adapter> adapter,
                           std::unique_ptr<PhysicalDevice> physicalDevice);
    explicit WebGPUAdapter(WebGPUInstance* wgpuInstance,
                           std::unique_ptr<Adapter> adapter,
                           std::unique_ptr<PhysicalDevice> physicalDevice,
                           WGPURequestAdapterOptions const* options);

public:
    virtual ~WebGPUAdapter() = default;

    WebGPUAdapter(const WebGPUAdapter&) = delete;
    WebGPUAdapter& operator=(const WebGPUAdapter&) = delete;

public: // WebGPU API
    void requestDevice(WGPUDeviceDescriptor const* descriptor, WGPURequestDeviceCallback callback, void* userdata);

public:
    std::shared_ptr<Adapter> getAdapter() const;
    PhysicalDevice* getPhysicalDevice() const;

private:
    [[maybe_unused]] WebGPUInstance* m_wgpuInstance = nullptr;
    [[maybe_unused]] const WGPURequestAdapterOptions m_options{};

private:
    std::shared_ptr<Adapter> m_adapter = nullptr; // shared with surface.
    std::unique_ptr<PhysicalDevice> m_physicalDevice = nullptr;
};

} // namespace jipu