#include "VulkanSwapchain.h"

#include "PhysicalDevice.h"

void Atlas::Swapchain::configureSwapchainBuilder(vkb::SwapchainBuilder& swapchainBuilder)
{
	swapchainBuilder.set_desired_format(VkSurfaceFormatKHR{ .format = mSwapchainImageFormat, .colorSpace = mColorSpace });
	swapchainBuilder.set_desired_present_mode(mPresentMode);
	swapchainBuilder.set_desired_extent(mWidth, mHeight);
	swapchainBuilder.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT);
}

Atlas::Swapchain::Swapchain(VkFormat swapchainImageFormat, EPresentMode presentMode, EColorSpace colorSpace, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	: mSwapchainImageFormat(swapchainImageFormat), mPresentMode(presentMode), mColorSpace(colorSpace), mDevice(device), mPhysicalDevice(physicalDevice), mSurface(surface)
{
}

Atlas::Swapchain::Swapchain(VkFormat swapchainImageFormat, EPresentMode presentMode, EColorSpace colorSpace)
	: Swapchain(swapchainImageFormat, presentMode, colorSpace, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) {
}

void Atlas::Swapchain::create(vkb::SwapchainBuilder swapchainBuilder) {
	const auto cSwapchainBuildResult = swapchainBuilder.build();
	
	vkb::Swapchain vkbSwapchain = cSwapchainBuildResult.value();

	mSwapchainExtent = vkbSwapchain.extent;
	//store swapchain and its related images

	setHandle(vkbSwapchain.swapchain);
	mSwapchainImages = vkbSwapchain.get_images().value();
	mSwapchainImageViews = vkbSwapchain.get_image_views().value();
}


void Atlas::Swapchain::create(uint32_t width, uint32_t height) {
	setWidth(width);
	setHeight(height);

	vkb::SwapchainBuilder swapchainBuilder{ mPhysicalDevice, mDevice, mSurface };
	configureSwapchainBuilder(swapchainBuilder);

	create(swapchainBuilder);
}
