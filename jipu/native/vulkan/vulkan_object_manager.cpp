#include "vulkan_object_manager.h"

#include "vulkan_device.h"

#include <spdlog/spdlog.h>

namespace jipu
{

std::unique_ptr<VulkanObjectManager> VulkanObjectManager::create(VulkanDevice* device)
{
    auto vulkanObjectManager = std::unique_ptr<VulkanObjectManager>(new VulkanObjectManager(device));

    return vulkanObjectManager;
}

VulkanObjectManager::VulkanObjectManager(VulkanDevice* device)
    : m_device(device)
{
    m_device->getInflightObjects()->subscribe(this, [this](VkFence fence, VulkanInflightObject object) {
        for (auto& commandBuffer : object.commandBuffers)
        {
            if (m_commandBuffers.contains(commandBuffer))
            {
                safeDestroy(commandBuffer);
            }
        }

        for (auto& buffer : object.buffers)
        {
            if (m_buffers.contains(buffer))
            {
                safeDestroy(buffer);
            }
        }

        for (auto& image : object.images)
        {
            if (m_images.contains(image))
            {
                safeDestroy(image);
            }
        }

        for (auto& imageView : object.imageViews)
        {
            if (m_imageViews.contains(imageView))
            {
                safeDestroy(imageView);
            }
        }

        for (auto& semaphore : object.signalSemaphores)
        {
            if (m_semaphores.contains(semaphore))
            {
                safeDestroy(semaphore);
            }
        }

        for (auto& sampler : object.samplers)
        {
            if (m_samplers.contains(sampler))
            {
                safeDestroy(sampler);
            }
        }

        for (auto& pipeline : object.pipelines)
        {
            if (m_pipelines.contains(pipeline))
            {
                safeDestroy(pipeline);
            }
        }

        for (auto& pipelineLayout : object.pipelineLayouts)
        {
            if (m_pipelineLayouts.contains(pipelineLayout))
            {
                safeDestroy(pipelineLayout);
            }
        }

        for (auto& descriptorSet : object.descriptorSet)
        {
            if (m_descriptorSets.contains(descriptorSet))
            {
                safeDestroy(descriptorSet);
            }
        }

        for (auto& descriptorSetLayout : object.descriptorSetLayouts)
        {
            if (m_descriptorSetLayouts.contains(descriptorSetLayout))
            {
                safeDestroy(descriptorSetLayout);
            }
        }

        for (auto& framebuffer : object.framebuffers)
        {
            if (m_framebuffers.contains(framebuffer))
            {
                safeDestroy(framebuffer);
            }
        }

        for (auto& renderPass : object.renderPasses)
        {
            if (m_renderPasses.contains(renderPass))
            {
                safeDestroy(renderPass);
            }
        }
    });
}

VulkanObjectManager::~VulkanObjectManager()
{
    m_device->getInflightObjects()->unsubscribe(this);

    for (auto buffer : m_buffers)
    {
        m_device->vkAPI.DestroyBuffer(m_device->getVkDevice(), buffer, nullptr);
    }

    for (auto image : m_images)
    {
        m_device->vkAPI.DestroyImage(m_device->getVkDevice(), image, nullptr);
    }

    for (auto commandBuffer : m_commandBuffers)
    {
        m_device->getCommandPool()->release(commandBuffer);
    }

    for (auto imageView : m_imageViews)
    {
        m_device->vkAPI.DestroyImageView(m_device->getVkDevice(), imageView, nullptr);
    }

    for (auto semaphore : m_semaphores)
    {
        m_device->getSemaphorePool()->release(semaphore);
    }

    for (auto sampler : m_samplers)
    {
        m_device->vkAPI.DestroySampler(m_device->getVkDevice(), sampler, nullptr);
    }

    for (auto pipeline : m_pipelines)
    {
        m_device->vkAPI.DestroyPipeline(m_device->getVkDevice(), pipeline, nullptr);
    }

    for (auto pipelineLayout : m_pipelineLayouts)
    {
        m_device->vkAPI.DestroyPipelineLayout(m_device->getVkDevice(), pipelineLayout, nullptr);
    }

    for (auto descriptorSet : m_descriptorSets)
    {
        m_device->vkAPI.FreeDescriptorSets(m_device->getVkDevice(), m_device->getVkDescriptorPool(), 1, &descriptorSet);
    }

    for (auto descriptorSetLayout : m_descriptorSetLayouts)
    {
        m_device->vkAPI.DestroyDescriptorSetLayout(m_device->getVkDevice(), descriptorSetLayout, nullptr);
    }

    for (auto framebuffer : m_framebuffers)
    {
        m_device->vkAPI.DestroyFramebuffer(m_device->getVkDevice(), framebuffer, nullptr);
    }

    for (auto renderPass : m_renderPasses)
    {
        m_device->vkAPI.DestroyRenderPass(m_device->getVkDevice(), renderPass, nullptr);
    }
}

void VulkanObjectManager::safeDestroy(VkBuffer buffer)
{
    if (m_device->getInflightObjects()->isInflight(buffer))
    {
        m_buffers.insert(buffer);
    }
    else
    {
        destroy(buffer);
    }
}

void VulkanObjectManager::safeDestroy(VkImage image)
{
    if (m_device->getInflightObjects()->isInflight(image))
    {
        m_images.insert(image);
    }
    else
    {
        m_images.erase(image);
        destroy(image);
    }
}

void VulkanObjectManager::safeDestroy(VkCommandBuffer commandBuffer)
{
    if (m_device->getInflightObjects()->isInflight(commandBuffer))
    {
        m_commandBuffers.insert(commandBuffer);
    }
    else
    {
        m_commandBuffers.erase(commandBuffer);
        destroy(commandBuffer);
    }
}

void VulkanObjectManager::safeDestroy(VkImageView imageView)
{
    if (m_device->getInflightObjects()->isInflight(imageView))
    {
        m_imageViews.insert(imageView);
    }
    else
    {
        m_imageViews.erase(imageView);
        destroy(imageView);
    }
}
void VulkanObjectManager::safeDestroy(VkSemaphore semaphore)
{
    if (m_device->getInflightObjects()->isInflight(semaphore))
    {
        m_semaphores.insert(semaphore);
    }
    else
    {
        m_semaphores.erase(semaphore);
        destroy(semaphore);
    }
}

void VulkanObjectManager::safeDestroy(VkSampler sampler)
{
    if (m_device->getInflightObjects()->isInflight(sampler))
    {
        m_samplers.insert(sampler);
    }
    else
    {
        m_samplers.erase(sampler);
        destroy(sampler);
    }
}
void VulkanObjectManager::safeDestroy(VkPipeline pipeline)
{
    if (m_device->getInflightObjects()->isInflight(pipeline))
    {
        m_pipelines.insert(pipeline);
    }
    else
    {
        m_pipelines.erase(pipeline);
        destroy(pipeline);
    }
}

void VulkanObjectManager::safeDestroy(VkPipelineLayout pipelineLayout)
{
    if (m_device->getInflightObjects()->isInflight(pipelineLayout))
    {
        m_pipelineLayouts.insert(pipelineLayout);
    }
    else
    {
        m_pipelineLayouts.erase(pipelineLayout);
        destroy(pipelineLayout);
    }
}

void VulkanObjectManager::safeDestroy(VkDescriptorSet descriptorSet)
{
    if (m_device->getInflightObjects()->isInflight(descriptorSet))
    {
        m_descriptorSets.insert(descriptorSet);
    }
    else
    {
        m_descriptorSets.erase(descriptorSet);
        destroy(descriptorSet);
    }
}

void VulkanObjectManager::safeDestroy(VkDescriptorSetLayout descriptorSetLayout)
{
    if (m_device->getInflightObjects()->isInflight(descriptorSetLayout))
    {
        m_descriptorSetLayouts.insert(descriptorSetLayout);
    }
    else
    {
        m_descriptorSetLayouts.erase(descriptorSetLayout);
        destroy(descriptorSetLayout);
    }
}

void VulkanObjectManager::safeDestroy(VkFramebuffer framebuffer)
{
    if (m_device->getInflightObjects()->isInflight(framebuffer))
    {
        spdlog::trace("framebuffer is inflight {}", reinterpret_cast<void*>(framebuffer));
        m_framebuffers.insert(framebuffer);
    }
    else
    {
        m_framebuffers.erase(framebuffer);
        destroy(framebuffer);
    }
}

void VulkanObjectManager::safeDestroy(VkRenderPass renderPass)
{
    if (m_device->getInflightObjects()->isInflight(renderPass))
    {
        m_renderPasses.insert(renderPass);
    }
    else
    {
        m_renderPasses.erase(renderPass);
        destroy(renderPass);
    }
}

void VulkanObjectManager::destroy(VkBuffer buffer)
{
    m_device->vkAPI.DestroyBuffer(m_device->getVkDevice(), buffer, nullptr);
}

void VulkanObjectManager::destroy(VkImage image)
{
    m_device->vkAPI.DestroyImage(m_device->getVkDevice(), image, nullptr);
}

void VulkanObjectManager::destroy(VkCommandBuffer commandBuffer)
{
    m_device->getCommandPool()->release(commandBuffer);
}

void VulkanObjectManager::destroy(VkImageView imageView)
{
    m_device->vkAPI.DestroyImageView(m_device->getVkDevice(), imageView, nullptr);
}
void VulkanObjectManager::destroy(VkSemaphore semaphore)
{
    m_device->getSemaphorePool()->release(semaphore);
}

void VulkanObjectManager::destroy(VkSampler sampler)
{
    m_device->vkAPI.DestroySampler(m_device->getVkDevice(), sampler, nullptr);
}
void VulkanObjectManager::destroy(VkPipeline pipeline)
{
    m_device->vkAPI.DestroyPipeline(m_device->getVkDevice(), pipeline, nullptr);
}

void VulkanObjectManager::destroy(VkPipelineLayout pipelineLayout)
{
    m_device->vkAPI.DestroyPipelineLayout(m_device->getVkDevice(), pipelineLayout, nullptr);
}

void VulkanObjectManager::destroy(VkDescriptorSet descriptorSet)
{
    m_device->vkAPI.FreeDescriptorSets(m_device->getVkDevice(), m_device->getVkDescriptorPool(), 1, &descriptorSet);
}

void VulkanObjectManager::destroy(VkDescriptorSetLayout descriptorSetLayout)
{
    m_device->vkAPI.DestroyDescriptorSetLayout(m_device->getVkDevice(), descriptorSetLayout, nullptr);
}

void VulkanObjectManager::destroy(VkFramebuffer framebuffer)
{
    spdlog::trace("framebuffer is destroyed {}", reinterpret_cast<void*>(framebuffer));
    m_device->vkAPI.DestroyFramebuffer(m_device->getVkDevice(), framebuffer, nullptr);
}

void VulkanObjectManager::destroy(VkRenderPass renderPass)
{
    m_device->vkAPI.DestroyRenderPass(m_device->getVkDevice(), renderPass, nullptr);
}

} // namespace jipu