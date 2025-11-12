#pragma once
#include <vector>
#include <iostream>
#include <fstream>

#include <vulkan/vulkan.h>
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace Atlas {
	VkCommandPoolCreateInfo CreateCommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
	VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo(VkCommandPool pool, uint32_t count = 1);

	VkFenceCreateInfo CreateFenceCreateInfo(VkFenceCreateFlags flags = 0 /*= 0*/);

	VkSemaphoreCreateInfo CreateSemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0 /*= 0*/);

	VkSemaphoreSubmitInfo SubmitSemaphoreInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);

	VkCommandBufferSubmitInfo SubmitCommandBufferInfo(VkCommandBuffer cmd);

	VkSubmitInfo2 SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
		VkSemaphoreSubmitInfo* waitSemaphoreInfo);

	VkCommandBufferBeginInfo CreateCommandBufferBeginInfo(VkCommandBufferUsageFlags flags /*= 0*/);

	void TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
	VkImageSubresourceRange CreateImageSubresourceRange(VkImageAspectFlags aspectMask);

	VkImageCreateInfo CreateImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);

	VkImageViewCreateInfo CreateImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);

	VkRenderingAttachmentInfo CreateAttachmentInfo(
		VkImageView view, VkClearValue* clear, VkImageLayout layout /*= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL*/);

	VkRenderingInfo CreateRenderingInfo(VkExtent2D renderExtent, VkRenderingAttachmentInfo* colorAttachment,
		VkRenderingAttachmentInfo* depthAttachment);
	void CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize);

	bool LoadShaderModule(const char* filePath,
	VkDevice device,
		VkShaderModule* outShaderModule);

	VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo();

	VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(VkShaderStageFlagBits stage,
		VkShaderModule shaderModule, const char* entry = "main");
}
