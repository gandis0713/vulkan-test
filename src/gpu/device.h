#pragma once

#include "gpu/pipeline.h"
#include "gpu/render_pass.h"
#include "gpu/swapchain.h"

#include <memory>

namespace vkt
{

class Adapter;

struct DeviceCreateInfo
{
};

class Device
{
public:
    Device() = delete;
    Device(Adapter* adapter, DeviceCreateInfo info);
    virtual ~Device() = default;

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    virtual std::unique_ptr<SwapChain> createSwapChain(SwapChainCreateInfo&& info) = 0;
    virtual std::unique_ptr<RenderPass> createRenderPass(RenderPassCreateInfo info) = 0;
    virtual std::unique_ptr<Pipeline> createPipeline(PipelineCreateInfo info) = 0;

protected:
    Adapter* m_adapter{ nullptr };
};

} // namespace vkt
