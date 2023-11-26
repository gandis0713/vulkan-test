#include "vulkan_binding_group.h"
#include "vulkan_binding_group_layout.h"
#include "vulkan_buffer.h"
#include "vulkan_device.h"
#include "vulkan_sampler.h"
#include "vulkan_texture.h"
#include "vulkan_texture_view.h"

#include <stdexcept>

namespace jipu
{

VulkanBindingGroup::VulkanBindingGroup(VulkanDevice* device, const BindingGroupDescriptor& descriptor)
    : BindingGroup(device, descriptor)
{
    auto descriptorSetLayout = downcast(descriptor.layout)->getVkDescriptorSetLayout();
    auto vulkanDevice = downcast(device);
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = downcast(device)->getVkDescriptorPool();
    descriptorSetAllocateInfo.descriptorSetCount = 1;
    descriptorSetAllocateInfo.pSetLayouts = &descriptorSetLayout;

    VkResult result = vkAPI.AllocateDescriptorSets(vulkanDevice->getVkDevice(), &descriptorSetAllocateInfo, &m_descriptorSet);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate descriptor sets.");
    }

    const uint64_t bufferSize = descriptor.buffers.size();
    const uint64_t samplerSize = descriptor.samplers.size();
    const uint64_t textureSize = descriptor.textures.size();

    std::vector<VkWriteDescriptorSet> descriptorWrites{};
    descriptorWrites.resize(bufferSize + samplerSize + textureSize);

    std::vector<VkDescriptorBufferInfo> bufferInfos{};
    bufferInfos.resize(bufferSize);
    for (auto i = 0; i < bufferSize; ++i)
    {
        const BufferBinding& buffer = descriptor.buffers[i];
        const BufferBindingLayout bufferLayout = descriptor.layout->getBufferBindingLayout(buffer.index);

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = downcast(buffer.buffer)->getVkBuffer();
        bufferInfo.offset = buffer.offset;
        bufferInfo.range = buffer.size;

        bufferInfos[i] = bufferInfo;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_descriptorSet;
        descriptorWrite.dstBinding = buffer.index;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = ToVkDescriptorType(bufferLayout.type, bufferLayout.dynamicOffset);
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = &bufferInfos[i];
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        descriptorWrites[i] = descriptorWrite;
    }

    std::vector<VkDescriptorImageInfo> samplerInfos{};
    samplerInfos.resize(samplerSize);
    // update sampler
    for (auto i = 0; i < samplerSize; ++i)
    {
        const SamplerBinding& sampler = descriptor.samplers[i];
        const SamplerBindingLayout samplerLayout = descriptor.layout->getSamplerBindingLayout(sampler.index);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.sampler = downcast(sampler.sampler)->getVkSampler();
        if (samplerLayout.withTexture)
        {
            imageInfo.imageView = downcast(sampler.textureView)->getVkImageView();
            imageInfo.imageLayout = downcast(sampler.textureView->getTexture())->getLayout();
        }

        samplerInfos[i] = imageInfo;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_descriptorSet;
        descriptorWrite.dstBinding = sampler.index;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = samplerLayout.withTexture ? VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER : VK_DESCRIPTOR_TYPE_SAMPLER;
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = nullptr;
        descriptorWrite.pImageInfo = &samplerInfos[i];
        descriptorWrite.pTexelBufferView = nullptr;

        descriptorWrites[bufferSize + i] = descriptorWrite;
    }

    std::vector<VkDescriptorImageInfo> textureInfos{};
    textureInfos.resize(textureSize);
    // update texture
    for (auto i = 0; i < textureSize; ++i)
    {
        const TextureBinding& texture = descriptor.textures[i];
        const TextureBindingLayout textureLayout = descriptor.layout->getTextureBindingLayout(texture.index);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = downcast(texture.textureView->getTexture())->getLayout();
        imageInfo.imageView = downcast(texture.textureView)->getVkImageView();

        textureInfos[i] = imageInfo;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_descriptorSet;
        descriptorWrite.dstBinding = texture.index;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = nullptr;
        descriptorWrite.pImageInfo = &textureInfos[i];
        descriptorWrite.pTexelBufferView = nullptr;

        descriptorWrites[bufferSize + samplerSize + i] = descriptorWrite;
    }

    vkAPI.UpdateDescriptorSets(vulkanDevice->getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

VulkanBindingGroup::~VulkanBindingGroup()
{
    auto vulkanDevice = downcast(m_device);
    const VulkanAPI& vkAPI = vulkanDevice->vkAPI;

    vkAPI.FreeDescriptorSets(vulkanDevice->getVkDevice(), vulkanDevice->getVkDescriptorPool(), 1, &m_descriptorSet);
}

VkDescriptorSet VulkanBindingGroup::getVkDescriptorSet() const
{
    return m_descriptorSet;
}

} // namespace jipu