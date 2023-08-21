#pragma once

#include "export.h"
#include "vkt/gpu/render_pass_encoder.h"
#include "vkt/gpu/texture.h"
#include <vector>

namespace vkt
{

class Pipeline;
class Buffer;
class CommandBuffer;
class TextureView;
class RenderPassEncoder;

struct BlitBuffer
{
    Buffer* buffer = nullptr;
    uint32_t offset = 0;
};

struct BlitTextureBuffer : BlitBuffer
{
    uint32_t bytesPerRow = 0;
    uint32_t rowsPerTexture = 0;
};

struct BlitTexture
{
    Texture* texture = nullptr;
};

struct CommandEncoderDescriptor
{
};

class VKT_EXPORT CommandEncoder
{
public:
    CommandEncoder() = delete;
    CommandEncoder(CommandBuffer* commandBuffer, const CommandEncoderDescriptor& descriptor);
    virtual ~CommandEncoder() = default;

    virtual std::unique_ptr<RenderPassEncoder> beginRenderPass(const RenderPassEncoderDescriptor& descriptor) = 0;

    virtual void copyBufferToBuffer(const BlitBuffer& src, const BlitBuffer& dst, uint64_t size) = 0;
    virtual void copyBufferToTexture(const BlitTextureBuffer& buffer, const BlitTexture& texture, const Extent3D& extent) = 0;

    virtual void copyTextureToBuffer() = 0;  // TODO: not yet implemented
    virtual void copyTextureToTexture() = 0; // TODO: not yet implemented

    virtual CommandBuffer* end() = 0;

public:
    CommandBuffer* getCommandBuffer() const;

protected:
    CommandBuffer* m_commandBuffer = nullptr;
};

} // namespace vkt