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

	class Image : public AVulkanHandleWrapper<VkImage> {
		friend class AllocatedImage;
	private:
		

		VkImageView mImageView = VK_NULL_HANDLE;
		VmaAllocation mAllocation = VK_NULL_HANDLE;
		VkExtent3D mImageExtent = { 0, 0, 0 };
		VkFormat mImageFormat = VK_FORMAT_UNDEFINED;

	protected:
		
		void createImage(GraphicsAllocationInfo& allocation, VkImageCreateInfo const* imageInfo) {
			vmaCreateImage(allocation.allocator, imageInfo, allocation.createInfo, getHandlePtr(), &mAllocation, nullptr);
		
		}

		void createImageView(std::shared_ptr<Device> device) {
			VkDevice deviceHandle = device->getHandle();

			VkImageViewCreateInfo imageViewInfo = CreateImageViewCreateInfo(getFormat(), getHandle(), VK_IMAGE_ASPECT_COLOR_BIT);
			vkCreateImageView(deviceHandle, &imageViewInfo, nullptr, &mImageView);
		}

	public:

		Image(GraphicsAllocationInfo allocation, VkExtent3D imageExtent, VkFormat imageFormat, VkImageUsageFlags usageFlags, bool bCreateImageView = true, std::shared_ptr<Device> device = Device::GetMainHandle())
			: AVulkanHandleWrapper(), mImageExtent(imageExtent), mImageFormat(imageFormat) {

			VkImageCreateInfo imageInfo = CreateImageCreateInfo(imageFormat, usageFlags, imageExtent);

			createImage(allocation, &imageInfo);
			
			if (bCreateImageView) {
				createImageView(device);
			}
		}

		Image() = default;

		VkImageView getImageView() {
			return mImageView;
		}

		VkExtent3D getExtent() const { 
			return mImageExtent;
		}

		VkFormat getFormat() const { return mImageFormat; }


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
