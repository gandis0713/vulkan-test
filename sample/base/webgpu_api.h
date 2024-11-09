#pragma once

#if defined(USE_DAWN_HEADER)
#include <dawn/webgpu.h>
#else
#include <webgpu.h>
#endif

namespace jipu
{

class DyLib;
struct WebGPUAPI
{
    bool loadProcs(DyLib* webgpuLib);

    WGPUProcCreateInstance CreateInstance = nullptr;
    // WGPUProcGetInstanceFeatures GetInstanceFeatures = nullptr;
    WGPUProcGetProcAddress GetProcAddress = nullptr;
    // WGPUProcAdapterGetFeatures AdapterGetFeatures = nullptr;
    // WGPUProcAdapterGetInfo AdapterGetInfo = nullptr;
    // WGPUProcAdapterGetLimits AdapterGetLimits = nullptr;
    // WGPUProcAdapterHasFeature AdapterHasFeature = nullptr;
    WGPUProcAdapterRequestDevice AdapterRequestDevice = nullptr;
    // WGPUProcAdapterAddRef AdapterAddRef = nullptr;
    WGPUProcAdapterRelease AdapterRelease = nullptr;
    // WGPUProcAdapterInfoFreeMembers AdapterInfoFreeMembers = nullptr;
    // WGPUProcBindGroupSetLabel BindGroupSetLabel = nullptr;
    // WGPUProcBindGroupAddRef BindGroupAddRef = nullptr;
    // WGPUProcBindGroupRelease BindGroupRelease = nullptr;
    // WGPUProcBindGroupLayoutSetLabel BindGroupLayoutSetLabel = nullptr;
    // WGPUProcBindGroupLayoutAddRef BindGroupLayoutAddRef = nullptr;
    // WGPUProcBindGroupLayoutRelease BindGroupLayoutRelease = nullptr;
    // WGPUProcBufferDestroy BufferDestroy = nullptr;
    // WGPUProcBufferGetConstMappedRange BufferGetConstMappedRange = nullptr;
    // WGPUProcBufferGetMapState BufferGetMapState = nullptr;
    WGPUProcBufferGetMappedRange BufferGetMappedRange = nullptr;
    // WGPUProcBufferGetSize BufferGetSize = nullptr;
    // WGPUProcBufferGetUsage BufferGetUsage = nullptr;
    // WGPUProcBufferMapAsync BufferMapAsync = nullptr;
    // WGPUProcBufferSetLabel BufferSetLabel = nullptr;
    WGPUProcBufferUnmap BufferUnmap = nullptr;
    // WGPUProcBufferAddRef BufferAddRef = nullptr;
    // WGPUProcBufferRelease BufferRelease = nullptr;
    // WGPUProcCommandBufferSetLabel CommandBufferSetLabel = nullptr;
    // WGPUProcCommandBufferAddRef CommandBufferAddRef = nullptr;
    WGPUProcCommandBufferRelease CommandBufferRelease = nullptr;
    // WGPUProcCommandEncoderBeginComputePass CommandEncoderBeginComputePass = nullptr;
    WGPUProcCommandEncoderBeginRenderPass CommandEncoderBeginRenderPass = nullptr;
    // WGPUProcCommandEncoderClearBuffer CommandEncoderClearBuffer = nullptr;
    // WGPUProcCommandEncoderCopyBufferToBuffer CommandEncoderCopyBufferToBuffer = nullptr;
    // WGPUProcCommandEncoderCopyBufferToTexture CommandEncoderCopyBufferToTexture = nullptr;
    // WGPUProcCommandEncoderCopyTextureToBuffer CommandEncoderCopyTextureToBuffer = nullptr;
    // WGPUProcCommandEncoderCopyTextureToTexture CommandEncoderCopyTextureToTexture = nullptr;
    WGPUProcCommandEncoderFinish CommandEncoderFinish = nullptr;
    // WGPUProcCommandEncoderInsertDebugMarker CommandEncoderInsertDebugMarker = nullptr;
    // WGPUProcCommandEncoderPopDebugGroup CommandEncoderPopDebugGroup = nullptr;
    // WGPUProcCommandEncoderPushDebugGroup CommandEncoderPushDebugGroup = nullptr;
    // WGPUProcCommandEncoderResolveQuerySet CommandEncoderResolveQuerySet = nullptr;
    // WGPUProcCommandEncoderSetLabel CommandEncoderSetLabel = nullptr;
    // WGPUProcCommandEncoderWriteTimestamp CommandEncoderWriteTimestamp = nullptr;
    // WGPUProcCommandEncoderAddRef CommandEncoderAddRef = nullptr;
    WGPUProcCommandEncoderRelease CommandEncoderRelease = nullptr;
    // WGPUProcComputePassEncoderDispatchWorkgroups ComputePassEncoderDispatchWorkgroups = nullptr;
    // WGPUProcComputePassEncoderDispatchWorkgroupsIndirect ComputePassEncoderDispatchWorkgroupsIndirect = nullptr;
    // WGPUProcComputePassEncoderEnd ComputePassEncoderEnd = nullptr;
    // WGPUProcComputePassEncoderInsertDebugMarker ComputePassEncoderInsertDebugMarker = nullptr;
    // WGPUProcComputePassEncoderPopDebugGroup ComputePassEncoderPopDebugGroup = nullptr;
    // WGPUProcComputePassEncoderPushDebugGroup ComputePassEncoderPushDebugGroup = nullptr;
    // WGPUProcComputePassEncoderSetBindGroup ComputePassEncoderSetBindGroup = nullptr;
    // WGPUProcComputePassEncoderSetLabel ComputePassEncoderSetLabel = nullptr;
    // WGPUProcComputePassEncoderSetPipeline ComputePassEncoderSetPipeline = nullptr;
    // WGPUProcComputePassEncoderAddRef ComputePassEncoderAddRef = nullptr;
    // WGPUProcComputePassEncoderRelease ComputePassEncoderRelease = nullptr;
    // WGPUProcComputePipelineGetBindGroupLayout ComputePipelineGetBindGroupLayout = nullptr;
    // WGPUProcComputePipelineSetLabel ComputePipelineSetLabel = nullptr;
    // WGPUProcComputePipelineAddRef ComputePipelineAddRef = nullptr;
    // WGPUProcComputePipelineRelease ComputePipelineRelease = nullptr;
    WGPUProcDeviceCreateBindGroup DeviceCreateBindGroup = nullptr;
    WGPUProcDeviceCreateBindGroupLayout DeviceCreateBindGroupLayout = nullptr;
    WGPUProcDeviceCreateBuffer DeviceCreateBuffer = nullptr;
    WGPUProcDeviceCreateCommandEncoder DeviceCreateCommandEncoder = nullptr;
    // WGPUProcDeviceCreateComputePipeline DeviceCreateComputePipeline = nullptr;
    // WGPUProcDeviceCreateComputePipelineAsync DeviceCreateComputePipelineAsync = nullptr;
    WGPUProcDeviceCreatePipelineLayout DeviceCreatePipelineLayout = nullptr;
    // WGPUProcDeviceCreateQuerySet DeviceCreateQuerySet = nullptr;
    // WGPUProcDeviceCreateRenderBundleEncoder DeviceCreateRenderBundleEncoder = nullptr;
    WGPUProcDeviceCreateRenderPipeline DeviceCreateRenderPipeline = nullptr;
    // WGPUProcDeviceCreateRenderPipelineAsync DeviceCreateRenderPipelineAsync = nullptr;
    // WGPUProcDeviceCreateSampler DeviceCreateSampler = nullptr;
    WGPUProcDeviceCreateShaderModule DeviceCreateShaderModule = nullptr;
    WGPUProcDeviceCreateTexture DeviceCreateTexture = nullptr;
    WGPUProcDeviceDestroy DeviceDestroy = nullptr;
    // WGPUProcDeviceGetFeatures DeviceGetFeatures = nullptr;
    // WGPUProcDeviceGetLimits DeviceGetLimits = nullptr;
    WGPUProcDeviceGetQueue DeviceGetQueue = nullptr;
    // WGPUProcDeviceHasFeature DeviceHasFeature = nullptr;
    // WGPUProcDevicePopErrorScope DevicePopErrorScope = nullptr;
    // WGPUProcDevicePushErrorScope DevicePushErrorScope = nullptr;
    // WGPUProcDeviceSetLabel DeviceSetLabel = nullptr;
    // WGPUProcDeviceAddRef DeviceAddRef = nullptr;
    WGPUProcDeviceRelease DeviceRelease = nullptr;
    WGPUProcInstanceCreateSurface InstanceCreateSurface = nullptr;
    // WGPUProcInstanceHasWGSLLanguageFeature InstanceHasWGSLLanguageFeature = nullptr;
    // WGPUProcInstanceProcessEvents InstanceProcessEvents = nullptr;
    WGPUProcInstanceRequestAdapter InstanceRequestAdapter = nullptr;
    // WGPUProcInstanceWaitAny InstanceWaitAny = nullptr;
    // WGPUProcInstanceAddRef InstanceAddRef = nullptr;
    WGPUProcInstanceRelease InstanceRelease = nullptr;
    // WGPUProcPipelineLayoutSetLabel PipelineLayoutSetLabel = nullptr;
    // WGPUProcPipelineLayoutAddRef PipelineLayoutAddRef = nullptr;
    WGPUProcPipelineLayoutRelease PipelineLayoutRelease = nullptr;
    // WGPUProcQuerySetDestroy QuerySetDestroy = nullptr;
    // WGPUProcQuerySetGetCount QuerySetGetCount = nullptr;
    // WGPUProcQuerySetGetType QuerySetGetType = nullptr;
    // WGPUProcQuerySetSetLabel QuerySetSetLabel = nullptr;
    // WGPUProcQuerySetAddRef QuerySetAddRef = nullptr;
    // WGPUProcQuerySetRelease QuerySetRelease = nullptr;
    // WGPUProcQueueOnSubmittedWorkDone QueueOnSubmittedWorkDone = nullptr;
    // WGPUProcQueueSetLabel QueueSetLabel = nullptr;
    WGPUProcQueueSubmit QueueSubmit = nullptr;
    // WGPUProcQueueWriteBuffer QueueWriteBuffer = nullptr;
    // WGPUProcQueueWriteTexture QueueWriteTexture = nullptr;
    // WGPUProcQueueAddRef QueueAddRef = nullptr;
    WGPUProcQueueRelease QueueRelease = nullptr;
    // WGPUProcRenderBundleSetLabel RenderBundleSetLabel = nullptr;
    // WGPUProcRenderBundleAddRef RenderBundleAddRef = nullptr;
    // WGPUProcRenderBundleRelease RenderBundleRelease = nullptr;
    // WGPUProcRenderBundleEncoderDraw RenderBundleEncoderDraw = nullptr;
    // WGPUProcRenderBundleEncoderDrawIndexed RenderBundleEncoderDrawIndexed = nullptr;
    // WGPUProcRenderBundleEncoderDrawIndexedIndirect RenderBundleEncoderDrawIndexedIndirect = nullptr;
    // WGPUProcRenderBundleEncoderDrawIndirect RenderBundleEncoderDrawIndirect = nullptr;
    // WGPUProcRenderBundleEncoderFinish RenderBundleEncoderFinish = nullptr;
    // WGPUProcRenderBundleEncoderInsertDebugMarker RenderBundleEncoderInsertDebugMarker = nullptr;
    // WGPUProcRenderBundleEncoderPopDebugGroup RenderBundleEncoderPopDebugGroup = nullptr;
    // WGPUProcRenderBundleEncoderPushDebugGroup RenderBundleEncoderPushDebugGroup = nullptr;
    // WGPUProcRenderBundleEncoderSetBindGroup RenderBundleEncoderSetBindGroup = nullptr;
    // WGPUProcRenderBundleEncoderSetIndexBuffer RenderBundleEncoderSetIndexBuffer = nullptr;
    // WGPUProcRenderBundleEncoderSetLabel RenderBundleEncoderSetLabel = nullptr;
    // WGPUProcRenderBundleEncoderSetPipeline RenderBundleEncoderSetPipeline = nullptr;
    // WGPUProcRenderBundleEncoderSetVertexBuffer RenderBundleEncoderSetVertexBuffer = nullptr;
    // WGPUProcRenderBundleEncoderAddRef RenderBundleEncoderAddRef = nullptr;
    // WGPUProcRenderBundleEncoderRelease RenderBundleEncoderRelease = nullptr;
    // WGPUProcRenderPassEncoderBeginOcclusionQuery RenderPassEncoderBeginOcclusionQuery = nullptr;
    WGPUProcRenderPassEncoderDraw RenderPassEncoderDraw = nullptr;
    // WGPUProcRenderPassEncoderDrawIndexed RenderPassEncoderDrawIndexed = nullptr;
    // WGPUProcRenderPassEncoderDrawIndexedIndirect RenderPassEncoderDrawIndexedIndirect = nullptr;
    // WGPUProcRenderPassEncoderDrawIndirect RenderPassEncoderDrawIndirect = nullptr;
    WGPUProcRenderPassEncoderEnd RenderPassEncoderEnd = nullptr;
    // WGPUProcRenderPassEncoderEndOcclusionQuery RenderPassEncoderEndOcclusionQuery = nullptr;
    // WGPUProcRenderPassEncoderExecuteBundles RenderPassEncoderExecuteBundles = nullptr;
    // WGPUProcRenderPassEncoderInsertDebugMarker RenderPassEncoderInsertDebugMarker = nullptr;
    // WGPUProcRenderPassEncoderPopDebugGroup RenderPassEncoderPopDebugGroup = nullptr;
    // WGPUProcRenderPassEncoderPushDebugGroup RenderPassEncoderPushDebugGroup = nullptr;
    // WGPUProcRenderPassEncoderSetBindGroup RenderPassEncoderSetBindGroup = nullptr;
    // WGPUProcRenderPassEncoderSetBlendConstant RenderPassEncoderSetBlendConstant = nullptr;
    // WGPUProcRenderPassEncoderSetIndexBuffer RenderPassEncoderSetIndexBuffer = nullptr;
    // WGPUProcRenderPassEncoderSetLabel RenderPassEncoderSetLabel = nullptr;
    WGPUProcRenderPassEncoderSetPipeline RenderPassEncoderSetPipeline = nullptr;
    // WGPUProcRenderPassEncoderSetScissorRect RenderPassEncoderSetScissorRect = nullptr;
    // WGPUProcRenderPassEncoderSetStencilReference RenderPassEncoderSetStencilReference = nullptr;
    // WGPUProcRenderPassEncoderSetVertexBuffer RenderPassEncoderSetVertexBuffer = nullptr;
    // WGPUProcRenderPassEncoderSetViewport RenderPassEncoderSetViewport = nullptr;
    // WGPUProcRenderPassEncoderAddRef RenderPassEncoderAddRef = nullptr;
    WGPUProcRenderPassEncoderRelease RenderPassEncoderRelease = nullptr;
    // WGPUProcRenderPipelineGetBindGroupLayout RenderPipelineGetBindGroupLayout = nullptr;
    // WGPUProcRenderPipelineSetLabel RenderPipelineSetLabel = nullptr;
    // WGPUProcRenderPipelineAddRef RenderPipelineAddRef = nullptr;
    WGPUProcRenderPipelineRelease RenderPipelineRelease = nullptr;
    // WGPUProcSamplerSetLabel SamplerSetLabel = nullptr;
    // WGPUProcSamplerAddRef SamplerAddRef = nullptr;
    // WGPUProcSamplerRelease SamplerRelease = nullptr;
    // WGPUProcShaderModuleGetCompilationInfo ShaderModuleGetCompilationInfo = nullptr;
    // WGPUProcShaderModuleSetLabel ShaderModuleSetLabel = nullptr;
    // WGPUProcShaderModuleAddRef ShaderModuleAddRef = nullptr;
    WGPUProcShaderModuleRelease ShaderModuleRelease = nullptr;
    // WGPUProcSupportedFeaturesFreeMembers SupportedFeaturesFreeMembers = nullptr;
    WGPUProcSurfaceConfigure SurfaceConfigure = nullptr;
    WGPUProcSurfaceGetCapabilities SurfaceGetCapabilities = nullptr;
    WGPUProcSurfaceGetCurrentTexture SurfaceGetCurrentTexture = nullptr;
    WGPUProcSurfacePresent SurfacePresent = nullptr;
    // WGPUProcSurfaceSetLabel SurfaceSetLabel = nullptr;
    // WGPUProcSurfaceUnconfigure SurfaceUnconfigure = nullptr;
    // WGPUProcSurfaceAddRef SurfaceAddRef = nullptr;
    WGPUProcSurfaceRelease SurfaceRelease = nullptr;
    // WGPUProcSurfaceCapabilitiesFreeMembers SurfaceCapabilitiesFreeMembers = nullptr;
    WGPUProcTextureCreateView TextureCreateView = nullptr;
    // WGPUProcTextureDestroy TextureDestroy = nullptr;
    // WGPUProcTextureGetDepthOrArrayLayers TextureGetDepthOrArrayLayers = nullptr;
    // WGPUProcTextureGetDimension TextureGetDimension = nullptr;
    // WGPUProcTextureGetFormat TextureGetFormat = nullptr;
    // WGPUProcTextureGetHeight TextureGetHeight = nullptr;
    // WGPUProcTextureGetMipLevelCount TextureGetMipLevelCount = nullptr;
    // WGPUProcTextureGetSampleCount TextureGetSampleCount = nullptr;
    // WGPUProcTextureGetUsage TextureGetUsage = nullptr;
    // WGPUProcTextureGetWidth TextureGetWidth = nullptr;
    // WGPUProcTextureSetLabel TextureSetLabel = nullptr;
    // WGPUProcTextureAddRef TextureAddRef = nullptr;
    WGPUProcTextureRelease TextureRelease = nullptr;
    // WGPUProcTextureViewSetLabel TextureViewSetLabel = nullptr;
    // WGPUProcTextureViewAddRef TextureViewAddRef = nullptr;
    WGPUProcTextureViewRelease TextureViewRelease = nullptr;
};

} // namespace jipu