/**************************************************************************************************
 * @file AllocatedImage.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include "../core/Core.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
	#include <vk_mem_alloc.h>
#endif

namespace Atlas {
	struct AllocatedImage {
		VkImage image = VK_NULL_HANDLE;
		VkImageView imageView = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		VkExtent3D imageExtent = { 0, 0, 0 };
		VkFormat imageFormat = VK_FORMAT_UNDEFINED;
	};
}
