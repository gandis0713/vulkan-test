#include "device_test.h"
#include <limits>

using namespace jipu;

TEST_F(DeviceTest, createSampler)
{
    SamplerDescriptor samplerDescriptor{};
    samplerDescriptor.minFilter = FilterMode::kLinear;
    samplerDescriptor.magFilter = FilterMode::kLinear;
    samplerDescriptor.addressModeU = AddressMode::kRepeat;
    samplerDescriptor.addressModeV = AddressMode::kRepeat;
    samplerDescriptor.addressModeW = AddressMode::kRepeat;
    samplerDescriptor.lodMin = 0;
    samplerDescriptor.lodMax = 32;
    auto sampler = m_device->createSampler(samplerDescriptor);
    ASSERT_NE(sampler, nullptr);
}

TEST_F(DeviceTest, createShaderModule)
{
    /*
    #version 450

    layout(location = 0) in vec2 inPos;
    layout(location = 1) in vec2 inUV;
    layout(location = 2) in vec4 inColor;

    layout(location = 0) out vec2 outUV;
    layout(location = 1) out vec4 outColor;

    layout(binding = 1) uniform UITransform {
        vec2 scale;
        vec2 translate;
    } uiTransform;

    out gl_PerVertex
    {
        vec4 gl_Position;
    };

    void main()
    {
        outUV = inUV;
        outColor = inColor;
        gl_Position = vec4(inPos * uiTransform.scale + uiTransform.translate, 0.0, 1.0);
    }
    */
    std::vector<uint32_t> vertexShaderSourceSpv = { 0x07230203, 0x00010000, 0x000d000b, 0x0000002b, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x000b000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x0000000b, 0x0000000f, 0x00000011, 0x00000015, 0x00000018, 0x00030003, 0x00000002, 0x000001c2, 0x000a0004, 0x475f4c47, 0x4c474f4f, 0x70635f45, 0x74735f70, 0x5f656c79, 0x656e696c, 0x7269645f, 0x69746365, 0x00006576, 0x00080004, 0x475f4c47, 0x4c474f4f, 0x6e695f45, 0x64756c63, 0x69645f65, 0x74636572, 0x00657669, 0x00040005, 0x00000004, 0x6e69616d, 0x00000000, 0x00040005, 0x00000009, 0x5574756f, 0x00000056, 0x00040005, 0x0000000b, 0x56556e69, 0x00000000, 0x00050005, 0x0000000f, 0x4374756f, 0x726f6c6f, 0x00000000, 0x00040005, 0x00000011, 0x6f436e69, 0x00726f6c, 0x00060005, 0x00000013, 0x505f6c67, 0x65567265, 0x78657472, 0x00000000, 0x00060006, 0x00000013, 0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69, 0x00030005, 0x00000015, 0x00000000, 0x00040005, 0x00000018, 0x6f506e69, 0x00000073, 0x00050005, 0x0000001a, 0x72544955, 0x66736e61, 0x006d726f, 0x00050006, 0x0000001a, 0x00000000, 0x6c616373, 0x00000065, 0x00060006, 0x0000001a, 0x00000001, 0x6e617274, 0x74616c73, 0x00000065, 0x00050005, 0x0000001c, 0x72546975, 0x66736e61, 0x006d726f, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00040047, 0x0000000b, 0x0000001e, 0x00000001, 0x00040047, 0x0000000f, 0x0000001e, 0x00000001, 0x00040047, 0x00000011, 0x0000001e, 0x00000002, 0x00050048, 0x00000013, 0x00000000, 0x0000000b, 0x00000000, 0x00030047, 0x00000013, 0x00000002, 0x00040047, 0x00000018, 0x0000001e, 0x00000000, 0x00050048, 0x0000001a, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000001a, 0x00000001, 0x00000023, 0x00000008, 0x00030047, 0x0000001a, 0x00000002, 0x00040047, 0x0000001c, 0x00000022, 0x00000000, 0x00040047, 0x0000001c, 0x00000021, 0x00000001, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000002, 0x00040020, 0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x00040020, 0x0000000a, 0x00000001, 0x00000007, 0x0004003b, 0x0000000a, 0x0000000b, 0x00000001, 0x00040017, 0x0000000d, 0x00000006, 0x00000004, 0x00040020, 0x0000000e, 0x00000003, 0x0000000d, 0x0004003b, 0x0000000e, 0x0000000f, 0x00000003, 0x00040020, 0x00000010, 0x00000001, 0x0000000d, 0x0004003b, 0x00000010, 0x00000011, 0x00000001, 0x0003001e, 0x00000013, 0x0000000d, 0x00040020, 0x00000014, 0x00000003, 0x00000013, 0x0004003b, 0x00000014, 0x00000015, 0x00000003, 0x00040015, 0x00000016, 0x00000020, 0x00000001, 0x0004002b, 0x00000016, 0x00000017, 0x00000000, 0x0004003b, 0x0000000a, 0x00000018, 0x00000001, 0x0004001e, 0x0000001a, 0x00000007, 0x00000007, 0x00040020, 0x0000001b, 0x00000002, 0x0000001a, 0x0004003b, 0x0000001b, 0x0000001c, 0x00000002, 0x00040020, 0x0000001d, 0x00000002, 0x00000007, 0x0004002b, 0x00000016, 0x00000021, 0x00000001, 0x0004002b, 0x00000006, 0x00000025, 0x00000000, 0x0004002b, 0x00000006, 0x00000026, 0x3f800000, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x00000007, 0x0000000c, 0x0000000b, 0x0003003e, 0x00000009, 0x0000000c, 0x0004003d, 0x0000000d, 0x00000012, 0x00000011, 0x0003003e, 0x0000000f, 0x00000012, 0x0004003d, 0x00000007, 0x00000019, 0x00000018, 0x00050041, 0x0000001d, 0x0000001e, 0x0000001c, 0x00000017, 0x0004003d, 0x00000007, 0x0000001f, 0x0000001e, 0x00050085, 0x00000007, 0x00000020, 0x00000019, 0x0000001f, 0x00050041, 0x0000001d, 0x00000022, 0x0000001c, 0x00000021, 0x0004003d, 0x00000007, 0x00000023, 0x00000022, 0x00050081, 0x00000007, 0x00000024, 0x00000020, 0x00000023, 0x00050051, 0x00000006, 0x00000027, 0x00000024, 0x00000000, 0x00050051, 0x00000006, 0x00000028, 0x00000024, 0x00000001, 0x00070050, 0x0000000d, 0x00000029, 0x00000027, 0x00000028, 0x00000025, 0x00000026, 0x00050041, 0x0000000e, 0x0000002a, 0x00000015, 0x00000017, 0x0003003e, 0x0000002a, 0x00000029, 0x000100fd, 0x00010038 };

    ShaderModuleDescriptor shaderModuleDescriptor{};
    shaderModuleDescriptor.code = reinterpret_cast<const char*>(vertexShaderSourceSpv.data());
    shaderModuleDescriptor.codeSize = static_cast<uint32_t>(vertexShaderSourceSpv.size() * 4);

    auto shaderModule = m_device->createShaderModule(shaderModuleDescriptor);
    ASSERT_NE(shaderModule, nullptr);
}
