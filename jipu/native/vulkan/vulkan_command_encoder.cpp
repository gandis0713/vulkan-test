#include "vulkan_command_encoder.h"

#include "vulkan_compute_pass_encoder.h"
#include "vulkan_device.h"
#include "vulkan_render_pass_encoder.h"

namespace jipu
{

VulkanCommandEncoder::VulkanCommandEncoder(VulkanDevice* device, const CommandEncoderDescriptor& descriptor)
    : m_device(device)
{
}

std::unique_ptr<ComputePassEncoder> VulkanCommandEncoder::beginComputePass(const ComputePassEncoderDescriptor& descriptor)
{
    return std::make_unique<VulkanComputePassEncoder>(this, descriptor);
}

std::unique_ptr<RenderPassEncoder> VulkanCommandEncoder::beginRenderPass(const RenderPassEncoderDescriptor& descriptor)
{
    return std::make_unique<VulkanRenderPassEncoder>(this, descriptor);
}

std::unique_ptr<RenderPassEncoder> VulkanCommandEncoder::beginRenderPass(const VulkanRenderPassEncoderDescriptor& descriptor)
{
    // TODO: encode begin render pass command
    return std::make_unique<VulkanRenderPassEncoder>(this, descriptor);
}

void VulkanCommandEncoder::copyBufferToBuffer(const CopyBuffer& src, const CopyBuffer& dst, uint64_t size)
{
    CopyBufferToBufferCommand command{
        { .type = CommandType::kCopyBufferToBuffer },
        src,
        dst,
        size
    };

    m_commandEncodingContext.commands.push_back(std::make_unique<CopyBufferToBufferCommand>(std::move(command)));
}

void VulkanCommandEncoder::copyBufferToTexture(const CopyTextureBuffer& buffer, const CopyTexture& texture, const Extent3D& extent)
{
    CopyBufferToTextureCommand command{
        { .type = CommandType::kCopyBufferToTexture },
        buffer,
        texture,
        extent
    };

    m_commandEncodingContext.commands.push_back(std::make_unique<CopyBufferToTextureCommand>(std::move(command)));
}

void VulkanCommandEncoder::copyTextureToBuffer(const CopyTexture& texture, const CopyTextureBuffer& buffer, const Extent3D& extent)
{
    CopyTextureToBufferCommand command{
        { .type = CommandType::kCopyTextureToBuffer },
        texture,
        buffer,
        extent
    };

    m_commandEncodingContext.commands.push_back(std::make_unique<CopyTextureToBufferCommand>(std::move(command)));
}

void VulkanCommandEncoder::copyTextureToTexture(const CopyTexture& src, const CopyTexture& dst, const Extent3D& extent)
{
    CopyTextureToTextureCommand command{
        { .type = CommandType::kCopyTextureToTexture },
        src,
        dst,
        extent
    };

    m_commandEncodingContext.commands.push_back(std::make_unique<CopyTextureToTextureCommand>(std::move(command)));
}

void VulkanCommandEncoder::resolveQuerySet(QuerySet* querySet,
                                           uint32_t firstQuery,
                                           uint32_t queryCount,
                                           Buffer* destination,
                                           uint64_t destinationOffset)
{

    ResolveQuerySetCommand command{
        { .type = CommandType::kResolveQuerySet },
        querySet,
        firstQuery,
        // queryCount,
        destination,
        destinationOffset
    };

    m_commandEncodingContext.commands.push_back(std::make_unique<ResolveQuerySetCommand>(std::move(command)));
}

std::unique_ptr<CommandBuffer> VulkanCommandEncoder::finish(const CommandBufferDescriptor& descriptor)
{
    return std::make_unique<VulkanCommandBuffer>(this, descriptor);
}

VulkanDevice* VulkanCommandEncoder::getDevice() const
{
    return m_device;
}

CommandEncodingContext& VulkanCommandEncoder::context()
{
    return m_commandEncodingContext;
}

CommandEncodingResult VulkanCommandEncoder::result()
{
    CommandEncodingResult result{};
    result.commands = std::move(m_commandEncodingContext.commands);
    result.operationResourceInfos = m_commandEncodingContext.commandResourceTracker.result();

    return result;
}

} // namespace jipu