#pragma once

#include <vector>
#include <memory>

#include "../core/Core.h"
#include "window/Window.h"
#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>

	#include <VkBootstrap.h>

	#include <vk_mem_alloc.h>
#endif


namespace Atlas {
#ifdef ATLAS_USE_VULKAN
	class VulkanSwapchain {
	private:
		VkColorSpaceKHR mSurfaceColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;

		std::vector<VkImage> mImages;
		std::vector<VkImageView> mImageViews;

		VkExtent2D mExtent;
		VkFormat mImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

		friend class VulkanRenderingBackend;

	public:

		//void init(const WindowDescription& cWindowDescription, VmaAllocator* allocator) {}
		
		void create(VkPhysicalDevice GPUDevice, VkDevice device, VkSurfaceKHR surface, const WindowDescription& cWindowDescription) {
			vkb::SwapchainBuilder swapchainBuilder{ GPUDevice, device, surface };

			//mImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

			vkb::Swapchain vkbSwapchain = swapchainBuilder
				//.use_default_format_selection()
				.set_desired_format(VkSurfaceFormatKHR{ .format = mImageFormat, .colorSpace = mSurfaceColorSpace })
				//use vsync present mode
				.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
				.set_desired_extent(cWindowDescription.windowRectangle.width, cWindowDescription.windowRectangle.height)
				.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
				.build()
				.value();

			mExtent = vkbSwapchain.extent;
			//store swapchain and its related images
			mSwapchain = vkbSwapchain.swapchain;
			mImages = vkbSwapchain.get_images().value();
			mImageViews = vkbSwapchain.get_image_views().value();
		}

		void setColorSpace(VkColorSpaceKHR colorSpace) {
			mSurfaceColorSpace = colorSpace;
		}

		void setImageFormat(VkFormat imageFormat) {
			mImageFormat = imageFormat;
		}
		

	};
#endif
}

