#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "../../core/Core.h"
#include "../window/Window.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>

	#include <VkBootstrap.h>

	#include <vk_mem_alloc.h>
#endif

#include <glm/glm.hpp>
#include "../RenderCommon.h"
#include "AllocatedImage.h"

namespace Atlas {
#ifdef ATLAS_USE_VULKAN
	using EPresentMode = VkPresentModeKHR;
	using EColorSpace = VkColorSpaceKHR;
	using EImageUsageFlags = VkImageUsageFlags;

	using SwapchainImages = std::vector<VkImage>;
	using SwapchainImageViews = std::vector<VkImageView>;

	class Swapchain : public VulkanGlobalStateObject<VkSwapchainKHR, Swapchain> {
		friend class VulkanRenderingBackend;
	private:
		VkFormat mSwapchainImageFormat{ VK_FORMAT_B8G8R8A8_UNORM };
		EPresentMode mPresentMode{ VK_PRESENT_MODE_FIFO_KHR };
		EColorSpace mColorSpace{ VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		SwapchainImages mSwapchainImages;
		SwapchainImageViews mSwapchainImageViews;

		VkExtent2D mSwapchainExtent;

		AllocatedImage mDrawImage;
		VkExtent2D mDrawExtent;

		VkDevice mDevice{ VK_NULL_HANDLE };
		VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
		VkSurfaceKHR mSurface{ VK_NULL_HANDLE };

		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };

		void configureSwapchainBuilder(vkb::SwapchainBuilder& swapchainBuilder);

	protected:

		void setWidth(uint32_t width) { mWidth = width; }
		void setHeight(uint32_t height) { mHeight = height; }

	public:

		Swapchain(VkFormat swapchainImageFormat, EPresentMode presentMode, EColorSpace colorSpace, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

		explicit Swapchain(VkFormat swapchainImageFormat, EPresentMode presentMode, EColorSpace colorSpace);

		Swapchain() = default;

		void create(vkb::SwapchainBuilder swapchainBuilder);

		void create(uint32_t width, uint32_t height);

		SwapchainImages getImages() const { return mSwapchainImages; }

		SwapchainImageViews getImageViews() const { return mSwapchainImageViews; }
	};
#endif
}

