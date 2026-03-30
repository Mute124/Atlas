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

#include "../../core/Core.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
	#include <vk_mem_alloc.h>

#endif

#include "../RenderCommon.h"
#include "../GraphicsUtils.h"

#include "PhysicalDevice.h"
#include <memory>

namespace Atlas {
	//using EImageFormat = VkFormat;

	class ImageView : public AVulkanHandleWrapper<VkImageView> {
	protected:
		bool createImageView(std::shared_ptr<Device> device, VkImageViewCreateInfo const* imageViewInfo) {
			VkDevice deviceHandle = device->getHandle();
			
			return vkCreateImageView(deviceHandle, imageViewInfo, nullptr, getHandlePtr()) == VK_SUCCESS;
		}

	public:
		ImageView(std::shared_ptr<Device> device, VkImageViewCreateInfo const* imageViewInfo) : AVulkanHandleWrapper() {

			ATLAS_ASSERT(imageViewInfo != nullptr, "Image view info must not be nullptr when creating an image view!");

			const bool cbCreateResult = createImageView(device, imageViewInfo);

			ATLAS_ASSERT(cbCreateResult, "Failed to create image view.");
			
			ImageView::setValidity(cbCreateResult);
			//vkCreateImageView(device->getHandle(), imageViewInfo, nullptr, getHandlePtr());
		}

		ImageView() = default;
	};

	struct ImageCreateInfo {
		VkFormat format{ VK_FORMAT_UNDEFINED };
		VkExtent3D extent { 0, 0, 0 };
		VkImageUsageFlags usageFlags;
		VmaAllocationCreateInfo allocationInfo{};

		ImageCreateInfo(const VkFormat& format, const VkExtent3D& extent, const VkImageUsageFlags& usageFlags, const VmaAllocationCreateInfo& allocationInfo)
			: format(format), extent(extent), usageFlags(usageFlags), allocationInfo(allocationInfo)
		{
		}

		ImageCreateInfo() = default;
	};

	class Image : public AVulkanHandleWrapper<VkImage> {
		friend class AllocatedImage;
	private:
		VkImageView mImageView{ VK_NULL_HANDLE };
		VmaAllocation mAllocation{ VK_NULL_HANDLE };
		VkExtent3D mImageExtent { 0, 0, 0 };
		VkFormat mImageFormat{ VK_FORMAT_UNDEFINED };
	public:

	};

	class AllocatedImage {
	public:

		VkImage image = VK_NULL_HANDLE;
		VkImageView imageView = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		VkExtent3D imageExtent = { 0, 0, 0 };
		VkFormat imageFormat = VK_FORMAT_UNDEFINED;

		AllocatedImage(GraphicsAllocationInfo allocation, VkExtent3D imageExtent, VkFormat imageFormat, VkImageUsageFlags usageFlags)
		: imageExtent(imageExtent), imageFormat(imageFormat) {

			VkImageCreateInfo imageInfo = CreateImageCreateInfo(imageFormat, usageFlags, imageExtent);
			
			//create(allocation, &imageInfo);
		}


		AllocatedImage() = default;
	};
}
