#include "webgpu_texture.h"

#include "jipu/native/texture.h"
#include "webgpu_device.h"
#include "webgpu_texture_view.h"

namespace jipu
{

WebGPUTexture* WebGPUTexture::create(WebGPUDevice* wgpuDevice, Texture* texture)
{
    return new WebGPUTexture(wgpuDevice, texture);
}

WebGPUTexture* WebGPUTexture::create(WebGPUDevice* device, WGPUTextureDescriptor const* descriptor)
{
    TextureDescriptor textureDescriptor{};
    textureDescriptor.type = WGPUToTextureType(descriptor->dimension);
    textureDescriptor.width = descriptor->size.width;
    textureDescriptor.height = descriptor->size.height;
    textureDescriptor.depth = descriptor->size.depthOrArrayLayers;
    textureDescriptor.mipLevels = descriptor->mipLevelCount;
    textureDescriptor.sampleCount = descriptor->sampleCount;
    textureDescriptor.format = WGPUToTextureFormat(descriptor->format);
    textureDescriptor.usage = WGPUToTextureUsageFlags(descriptor->usage, descriptor->format);

    auto texture = device->getDevice()->createTexture(textureDescriptor);

    return new WebGPUTexture(device, std::move(texture), descriptor);
}

WebGPUTexture::WebGPUTexture(WebGPUDevice* device, Texture* texture)
    : m_wgpuDevice(device)
    , m_externalTexture(texture)
{
}

WebGPUTexture::WebGPUTexture(WebGPUDevice* device, std::unique_ptr<Texture> texture, WGPUTextureDescriptor const* descriptor)
    : m_wgpuDevice(device)
    , m_descriptor(*descriptor)
    , m_ownTexture(std::move(texture))
{
}

WebGPUTextureView* WebGPUTexture::createView(WGPUTextureViewDescriptor const* descriptor)
{
    return WebGPUTextureView::create(this, descriptor);
}

Texture* WebGPUTexture::getTexture() const
{
    return m_externalTexture ? m_externalTexture : m_ownTexture.get();
}

// Convert from JIPU to WebGPU
WGPUTextureFormat ToWGPUTextureFormat(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::kUndefined:
        return WGPUTextureFormat::WGPUTextureFormat_Undefined;
    case TextureFormat::kR8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_R8Unorm;
    case TextureFormat::kR8Snorm:
        return WGPUTextureFormat::WGPUTextureFormat_R8Snorm;
    case TextureFormat::kR8Uint:
        return WGPUTextureFormat::WGPUTextureFormat_R8Uint;
    case TextureFormat::kR8Sint:
        return WGPUTextureFormat::WGPUTextureFormat_R8Sint;
    case TextureFormat::kR16Uint:
        return WGPUTextureFormat::WGPUTextureFormat_R16Uint;
    case TextureFormat::kR16Sint:
        return WGPUTextureFormat::WGPUTextureFormat_R16Sint;
    case TextureFormat::kR16Float:
        return WGPUTextureFormat::WGPUTextureFormat_R16Float;
    case TextureFormat::kRG8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_RG8Unorm;
    case TextureFormat::kRG8Snorm:
        return WGPUTextureFormat::WGPUTextureFormat_RG8Snorm;
    case TextureFormat::kRG8Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RG8Uint;
    case TextureFormat::kRG8Sint:
        return WGPUTextureFormat::WGPUTextureFormat_RG8Sint;
    case TextureFormat::kR32Float:
        return WGPUTextureFormat::WGPUTextureFormat_R32Float;
    case TextureFormat::kR32Uint:
        return WGPUTextureFormat::WGPUTextureFormat_R32Uint;
    case TextureFormat::kR32Sint:
        return WGPUTextureFormat::WGPUTextureFormat_R32Sint;
    case TextureFormat::kRG16Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RG16Uint;
    case TextureFormat::kRG16Sint:
        return WGPUTextureFormat::WGPUTextureFormat_RG16Sint;
    case TextureFormat::kRG16Float:
        return WGPUTextureFormat::WGPUTextureFormat_RG16Float;
    case TextureFormat::kRGBA8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA8Unorm;
    case TextureFormat::kRGBA8UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA8UnormSrgb;
    case TextureFormat::kRGBA8Snorm:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA8Snorm;
    case TextureFormat::kRGBA8Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA8Uint;
    case TextureFormat::kRGBA8Sint:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA8Sint;
    case TextureFormat::kBGRA8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_BGRA8Unorm;
    case TextureFormat::kBGRA8UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_BGRA8UnormSrgb;
    case TextureFormat::kRGB10A2Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RGB10A2Uint;
    case TextureFormat::kRGB10A2Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_RGB10A2Unorm;
    case TextureFormat::kRG11B10Ufloat:
        return WGPUTextureFormat::WGPUTextureFormat_RG11B10Ufloat;
    case TextureFormat::kRGB9E5Ufloat:
        return WGPUTextureFormat::WGPUTextureFormat_RGB9E5Ufloat;
    case TextureFormat::kRG32Float:
        return WGPUTextureFormat::WGPUTextureFormat_RG32Float;
    case TextureFormat::kRG32Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RG32Uint;
    case TextureFormat::kRG32Sint:
        return WGPUTextureFormat::WGPUTextureFormat_RG32Sint;
    case TextureFormat::kRGBA16Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA16Uint;
    case TextureFormat::kRGBA16Sint:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA16Sint;
    case TextureFormat::kRGBA16Float:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA16Float;
    case TextureFormat::kRGBA32Float:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA32Float;
    case TextureFormat::kRGBA32Uint:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA32Uint;
    case TextureFormat::kRGBA32Sint:
        return WGPUTextureFormat::WGPUTextureFormat_RGBA32Sint;
    case TextureFormat::kStencil8:
        return WGPUTextureFormat::WGPUTextureFormat_Stencil8;
    case TextureFormat::kDepth16Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_Depth16Unorm;
    case TextureFormat::kDepth24Plus:
        return WGPUTextureFormat::WGPUTextureFormat_Depth24Plus;
    case TextureFormat::kDepth24PlusStencil8:
        return WGPUTextureFormat::WGPUTextureFormat_Depth24PlusStencil8;
    case TextureFormat::kDepth32Float:
        return WGPUTextureFormat::WGPUTextureFormat_Depth32Float;
    case TextureFormat::kBC1RGBAUnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC1RGBAUnorm;
    case TextureFormat::kBC1RGBAUnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_BC1RGBAUnormSrgb;
    case TextureFormat::kBC2RGBAUnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC2RGBAUnorm;
    case TextureFormat::kBC2RGBAUnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_BC2RGBAUnormSrgb;
    case TextureFormat::kBC3RGBAUnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC3RGBAUnorm;
    case TextureFormat::kBC3RGBAUnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_BC3RGBAUnormSrgb;
    case TextureFormat::kBC4RUnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC4RUnorm;
    case TextureFormat::kBC4RSnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC4RSnorm;
    case TextureFormat::kBC5RGUnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC5RGUnorm;
    case TextureFormat::kBC5RGSnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC5RGSnorm;
    case TextureFormat::kBC6HRGBUfloat:
        return WGPUTextureFormat::WGPUTextureFormat_BC6HRGBUfloat;
    case TextureFormat::kBC6HRGBFloat:
        return WGPUTextureFormat::WGPUTextureFormat_BC6HRGBFloat;
    case TextureFormat::kBC7RGBAUnorm:
        return WGPUTextureFormat::WGPUTextureFormat_BC7RGBAUnorm;
    case TextureFormat::kBC7RGBAUnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_BC7RGBAUnormSrgb;
    case TextureFormat::kETC2RGB8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8Unorm;
    case TextureFormat::kETC2RGB8UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8UnormSrgb;
    case TextureFormat::kETC2RGB8A1Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8A1Unorm;
    case TextureFormat::kETC2RGB8A1UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8A1UnormSrgb;
    case TextureFormat::kETC2RGBA8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ETC2RGBA8Unorm;
    case TextureFormat::kETC2RGBA8UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ETC2RGBA8UnormSrgb;
    case TextureFormat::kEACR11Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_EACR11Unorm;
    case TextureFormat::kEACR11Snorm:
        return WGPUTextureFormat::WGPUTextureFormat_EACR11Snorm;
    case TextureFormat::kEACRG11Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_EACRG11Unorm;
    case TextureFormat::kEACRG11Snorm:
        return WGPUTextureFormat::WGPUTextureFormat_EACRG11Snorm;
    case TextureFormat::kASTC4x4Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC4x4Unorm;
    case TextureFormat::kASTC4x4UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC4x4UnormSrgb;
    case TextureFormat::kASTC5x4Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC5x4Unorm;
    case TextureFormat::kASTC5x4UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC5x4UnormSrgb;
    case TextureFormat::kASTC5x5Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC5x5Unorm;
    case TextureFormat::kASTC5x5UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC5x5UnormSrgb;
    case TextureFormat::kASTC6x5Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC6x5Unorm;
    case TextureFormat::kASTC6x5UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC6x5UnormSrgb;
    case TextureFormat::kASTC6x6Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC6x6Unorm;
    case TextureFormat::kASTC6x6UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC6x6UnormSrgb;
    case TextureFormat::kASTC8x5Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC8x5Unorm;
    case TextureFormat::kASTC8x5UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC8x5UnormSrgb;
    case TextureFormat::kASTC8x6Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC8x6Unorm;
    case TextureFormat::kASTC8x6UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC8x6UnormSrgb;
    case TextureFormat::kASTC8x8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC8x8Unorm;
    case TextureFormat::kASTC8x8UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC8x8UnormSrgb;
    case TextureFormat::kASTC10x5Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x5Unorm;
    case TextureFormat::kASTC10x5UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x5UnormSrgb;
    case TextureFormat::kASTC10x6Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x6Unorm;
    case TextureFormat::kASTC10x6UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x6UnormSrgb;
    case TextureFormat::kASTC10x8Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x8Unorm;
    case TextureFormat::kASTC10x8UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x8UnormSrgb;
    case TextureFormat::kASTC10x10Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x10Unorm;
    case TextureFormat::kASTC10x10UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC10x10UnormSrgb;
    case TextureFormat::kASTC12x10Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC12x10Unorm;
    case TextureFormat::kASTC12x10UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC12x10UnormSrgb;
    case TextureFormat::kASTC12x12Unorm:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC12x12Unorm;
    case TextureFormat::kASTC12x12UnormSrgb:
        return WGPUTextureFormat::WGPUTextureFormat_ASTC12x12UnormSrgb;
    // case TextureFormat::kR16Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R16Unorm;
    // case TextureFormat::kRG16Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_RG16Unorm;
    // case TextureFormat::kRGBA16Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_RGBA16Unorm;
    // case TextureFormat::kR16Snorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R16Snorm;
    // case TextureFormat::kRG16Snorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_RG16Snorm;
    // case TextureFormat::kRGBA16Snorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_RGBA16Snorm;
    // case TextureFormat::kR8BG8Biplanar420Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R8BG8Biplanar420Unorm;
    // case TextureFormat::kR10X6BG10X6Biplanar420Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R10X6BG10X6Biplanar420Unorm;
    // case TextureFormat::kR8BG8A8Triplanar420Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R8BG8A8Triplanar420Unorm;
    // case TextureFormat::kR8BG8Biplanar422Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R8BG8Biplanar422Unorm;
    // case TextureFormat::kR8BG8Biplanar444Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R8BG8Biplanar444Unorm;
    // case TextureFormat::kR10X6BG10X6Biplanar422Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R10X6BG10X6Biplanar422Unorm;
    // case TextureFormat::kR10X6BG10X6Biplanar444Unorm:
    //     return WGPUTextureFormat::WGPUTextureFormat_R10X6BG10X6Biplanar444Unorm;
    // case TextureFormat::kExternal:
    //     return WGPUTextureFormat::WGPUTextureFormat_External;
    case TextureFormat::kForce32:
        return WGPUTextureFormat::WGPUTextureFormat_Force32;
    default:
        return WGPUTextureFormat::WGPUTextureFormat_Undefined;
    }
}

WGPUTextureDimension ToWGPUTextureDimension(TextureType type)
{
    switch (type)
    {
    case TextureType::k1D:
        return WGPUTextureDimension::WGPUTextureDimension_1D;
    default:
    case TextureType::k2D:
        return WGPUTextureDimension::WGPUTextureDimension_2D;
    case TextureType::k3D:
        return WGPUTextureDimension::WGPUTextureDimension_3D;
    }
}

WGPUTextureUsageFlags ToWGPUTextureUsageFlags(TextureUsageFlags flags)
{
    WGPUTextureUsageFlags wgpuUsages = WGPUTextureUsage_None;
    if (flags & TextureUsageFlagBits::kCopySrc)
    {
        wgpuUsages |= WGPUTextureUsage_CopySrc;
    }
    if (flags & TextureUsageFlagBits::kCopyDst)
    {
        wgpuUsages |= WGPUTextureUsage_CopyDst;
    }
    if (flags & TextureUsageFlagBits::kTextureBinding)
    {
        wgpuUsages |= WGPUTextureUsage_TextureBinding;
    }
    if (flags & TextureUsageFlagBits::kStorageBinding)
    {
        wgpuUsages |= WGPUTextureUsage_StorageBinding;
    }
    if (flags & TextureUsageFlagBits::kRenderAttachment)
    {
        wgpuUsages |= WGPUTextureUsage_RenderAttachment;
    }

    return wgpuUsages;
}

// Convert from WebGPU to JIPU
TextureFormat WGPUToTextureFormat(WGPUTextureFormat format)
{
    switch (format)
    {
    case WGPUTextureFormat::WGPUTextureFormat_Undefined:
        return TextureFormat::kUndefined;
    case WGPUTextureFormat::WGPUTextureFormat_R8Unorm:
        return TextureFormat::kR8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_R8Snorm:
        return TextureFormat::kR8Snorm;
    case WGPUTextureFormat::WGPUTextureFormat_R8Uint:
        return TextureFormat::kR8Uint;
    case WGPUTextureFormat::WGPUTextureFormat_R8Sint:
        return TextureFormat::kR8Sint;
    case WGPUTextureFormat::WGPUTextureFormat_R16Uint:
        return TextureFormat::kR16Uint;
    case WGPUTextureFormat::WGPUTextureFormat_R16Sint:
        return TextureFormat::kR16Sint;
    case WGPUTextureFormat::WGPUTextureFormat_R16Float:
        return TextureFormat::kR16Float;
    case WGPUTextureFormat::WGPUTextureFormat_RG8Unorm:
        return TextureFormat::kRG8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_RG8Snorm:
        return TextureFormat::kRG8Snorm;
    case WGPUTextureFormat::WGPUTextureFormat_RG8Uint:
        return TextureFormat::kRG8Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RG8Sint:
        return TextureFormat::kRG8Sint;
    case WGPUTextureFormat::WGPUTextureFormat_R32Float:
        return TextureFormat::kR32Float;
    case WGPUTextureFormat::WGPUTextureFormat_R32Uint:
        return TextureFormat::kR32Uint;
    case WGPUTextureFormat::WGPUTextureFormat_R32Sint:
        return TextureFormat::kR32Sint;
    case WGPUTextureFormat::WGPUTextureFormat_RG16Uint:
        return TextureFormat::kRG16Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RG16Sint:
        return TextureFormat::kRG16Sint;
    case WGPUTextureFormat::WGPUTextureFormat_RG16Float:
        return TextureFormat::kRG16Float;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA8Unorm:
        return TextureFormat::kRGBA8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA8UnormSrgb:
        return TextureFormat::kRGBA8UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA8Snorm:
        return TextureFormat::kRGBA8Snorm;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA8Uint:
        return TextureFormat::kRGBA8Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA8Sint:
        return TextureFormat::kRGBA8Sint;
    case WGPUTextureFormat::WGPUTextureFormat_BGRA8Unorm:
        return TextureFormat::kBGRA8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_BGRA8UnormSrgb:
        return TextureFormat::kBGRA8UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_RGB10A2Uint:
        return TextureFormat::kRGB10A2Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RGB10A2Unorm:
        return TextureFormat::kRGB10A2Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_RG11B10Ufloat:
        return TextureFormat::kRG11B10Ufloat;
    case WGPUTextureFormat::WGPUTextureFormat_RGB9E5Ufloat:
        return TextureFormat::kRGB9E5Ufloat;
    case WGPUTextureFormat::WGPUTextureFormat_RG32Float:
        return TextureFormat::kRG32Float;
    case WGPUTextureFormat::WGPUTextureFormat_RG32Uint:
        return TextureFormat::kRG32Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RG32Sint:
        return TextureFormat::kRG32Sint;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA16Uint:
        return TextureFormat::kRGBA16Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA16Sint:
        return TextureFormat::kRGBA16Sint;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA16Float:
        return TextureFormat::kRGBA16Float;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA32Float:
        return TextureFormat::kRGBA32Float;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA32Uint:
        return TextureFormat::kRGBA32Uint;
    case WGPUTextureFormat::WGPUTextureFormat_RGBA32Sint:
        return TextureFormat::kRGBA32Sint;
    case WGPUTextureFormat::WGPUTextureFormat_Stencil8:
        return TextureFormat::kStencil8;
    case WGPUTextureFormat::WGPUTextureFormat_Depth16Unorm:
        return TextureFormat::kDepth16Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_Depth24Plus:
        return TextureFormat::kDepth24Plus;
    case WGPUTextureFormat::WGPUTextureFormat_Depth24PlusStencil8:
        return TextureFormat::kDepth24PlusStencil8;
    case WGPUTextureFormat::WGPUTextureFormat_Depth32Float:
        return TextureFormat::kDepth32Float;
    case WGPUTextureFormat::WGPUTextureFormat_BC1RGBAUnorm:
        return TextureFormat::kBC1RGBAUnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC1RGBAUnormSrgb:
        return TextureFormat::kBC1RGBAUnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_BC2RGBAUnorm:
        return TextureFormat::kBC2RGBAUnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC2RGBAUnormSrgb:
        return TextureFormat::kBC2RGBAUnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_BC3RGBAUnorm:
        return TextureFormat::kBC3RGBAUnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC3RGBAUnormSrgb:
        return TextureFormat::kBC3RGBAUnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_BC4RUnorm:
        return TextureFormat::kBC4RUnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC4RSnorm:
        return TextureFormat::kBC4RSnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC5RGUnorm:
        return TextureFormat::kBC5RGUnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC5RGSnorm:
        return TextureFormat::kBC5RGSnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC6HRGBUfloat:
        return TextureFormat::kBC6HRGBUfloat;
    case WGPUTextureFormat::WGPUTextureFormat_BC6HRGBFloat:
        return TextureFormat::kBC6HRGBFloat;
    case WGPUTextureFormat::WGPUTextureFormat_BC7RGBAUnorm:
        return TextureFormat::kBC7RGBAUnorm;
    case WGPUTextureFormat::WGPUTextureFormat_BC7RGBAUnormSrgb:
        return TextureFormat::kBC7RGBAUnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8Unorm:
        return TextureFormat::kETC2RGB8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8UnormSrgb:
        return TextureFormat::kETC2RGB8UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8A1Unorm:
        return TextureFormat::kETC2RGB8A1Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ETC2RGB8A1UnormSrgb:
        return TextureFormat::kETC2RGB8A1UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ETC2RGBA8Unorm:
        return TextureFormat::kETC2RGBA8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ETC2RGBA8UnormSrgb:
        return TextureFormat::kETC2RGBA8UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_EACR11Unorm:
        return TextureFormat::kEACR11Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_EACR11Snorm:
        return TextureFormat::kEACR11Snorm;
    case WGPUTextureFormat::WGPUTextureFormat_EACRG11Unorm:
        return TextureFormat::kEACRG11Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_EACRG11Snorm:
        return TextureFormat::kEACRG11Snorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC4x4Unorm:
        return TextureFormat::kASTC4x4Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC4x4UnormSrgb:
        return TextureFormat::kASTC4x4UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC5x4Unorm:
        return TextureFormat::kASTC5x4Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC5x4UnormSrgb:
        return TextureFormat::kASTC5x4UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC5x5Unorm:
        return TextureFormat::kASTC5x5Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC5x5UnormSrgb:
        return TextureFormat::kASTC5x5UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC6x5Unorm:
        return TextureFormat::kASTC6x5Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC6x5UnormSrgb:
        return TextureFormat::kASTC6x5UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC6x6Unorm:
        return TextureFormat::kASTC6x6Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC6x6UnormSrgb:
        return TextureFormat::kASTC6x6UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC8x5Unorm:
        return TextureFormat::kASTC8x5Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC8x5UnormSrgb:
        return TextureFormat::kASTC8x5UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC8x6Unorm:
        return TextureFormat::kASTC8x6Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC8x6UnormSrgb:
        return TextureFormat::kASTC8x6UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC8x8Unorm:
        return TextureFormat::kASTC8x8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC8x8UnormSrgb:
        return TextureFormat::kASTC8x8UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x5Unorm:
        return TextureFormat::kASTC10x5Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x5UnormSrgb:
        return TextureFormat::kASTC10x5UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x6Unorm:
        return TextureFormat::kASTC10x6Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x6UnormSrgb:
        return TextureFormat::kASTC10x6UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x8Unorm:
        return TextureFormat::kASTC10x8Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x8UnormSrgb:
        return TextureFormat::kASTC10x8UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x10Unorm:
        return TextureFormat::kASTC10x10Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC10x10UnormSrgb:
        return TextureFormat::kASTC10x10UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC12x10Unorm:
        return TextureFormat::kASTC12x10Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC12x10UnormSrgb:
        return TextureFormat::kASTC12x10UnormSrgb;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC12x12Unorm:
        return TextureFormat::kASTC12x12Unorm;
    case WGPUTextureFormat::WGPUTextureFormat_ASTC12x12UnormSrgb:
        return TextureFormat::kASTC12x12UnormSrgb;
    // case WGPUTextureFormat::WGPUTextureFormat_R16Unorm:
    //     return TextureFormat::kR16Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_RG16Unorm:
    //     return TextureFormat::kRG16Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_RGBA16Unorm:
    //     return TextureFormat::kRGBA16Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R16Snorm:
    //     return TextureFormat::kR16Snorm;
    // case WGPUTextureFormat::WGPUTextureFormat_RG16Snorm:
    //     return TextureFormat::kRG16Snorm;
    // case WGPUTextureFormat::WGPUTextureFormat_RGBA16Snorm:
    //     return TextureFormat::kRGBA16Snorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R8BG8Biplanar420Unorm:
    //     return TextureFormat::kR8BG8Biplanar420Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R10X6BG10X6Biplanar420Unorm:
    //     return TextureFormat::kR10X6BG10X6Biplanar420Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R8BG8A8Triplanar420Unorm:
    //     return TextureFormat::kR8BG8A8Triplanar420Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R8BG8Biplanar422Unorm:
    //     return TextureFormat::kR8BG8Biplanar422Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R8BG8Biplanar444Unorm:
    //     return TextureFormat::kR8BG8Biplanar444Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R10X6BG10X6Biplanar422Unorm:
    //     return TextureFormat::kR10X6BG10X6Biplanar422Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_R10X6BG10X6Biplanar444Unorm:
    //     return TextureFormat::kR10X6BG10X6Biplanar444Unorm;
    // case WGPUTextureFormat::WGPUTextureFormat_External:
    //     return TextureFormat::kExternal;
    case WGPUTextureFormat::WGPUTextureFormat_Force32:
        return TextureFormat::kForce32;
    default:
        return TextureFormat::kUndefined;
    }
}

TextureType WGPUToTextureType(WGPUTextureDimension dimension)
{
    switch (dimension)
    {
    case WGPUTextureDimension::WGPUTextureDimension_1D:
        return TextureType::k1D;
    default:
    case WGPUTextureDimension::WGPUTextureDimension_2D:
        return TextureType::k2D;
    case WGPUTextureDimension::WGPUTextureDimension_3D:
        return TextureType::k3D;
    }
}

TextureUsageFlags WGPUToTextureUsageFlags(WGPUTextureUsageFlags flags, WGPUTextureFormat format)
{
    TextureUsageFlags jipuUsageFlags = TextureUsageFlagBits::kUndefined;
    if (flags & WGPUTextureUsage_CopySrc)
    {
        jipuUsageFlags |= TextureUsageFlagBits::kCopySrc;
    }
    if (flags & WGPUTextureUsage_CopyDst)
    {
        jipuUsageFlags |= TextureUsageFlagBits::kCopyDst;
    }
    if (flags & WGPUTextureUsage_TextureBinding)
    {
        jipuUsageFlags |= TextureUsageFlagBits::kTextureBinding;
    }
    if (flags & WGPUTextureUsage_StorageBinding)
    {
        jipuUsageFlags |= TextureUsageFlagBits::kStorageBinding;
    }
    if (flags & WGPUTextureUsage_RenderAttachment)
    {
        jipuUsageFlags |= TextureUsageFlagBits::kRenderAttachment;
    }

    return jipuUsageFlags;
}

} // namespace jipu