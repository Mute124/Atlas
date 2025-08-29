#include "VKDevice.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan_core.h>
	#include <VkBootstrap.h>

	#ifdef ATLAS_USE_SDL2
		#include <SDL_vulkan.h>
	#endif

#endif


#ifdef ATLAS_USE_VULKAN

void Atlas::VulkanRenderingBackend::setApplicationName(std::string_view applicationName) {
	mApplicationName = applicationName;
}

std::string Atlas::VulkanRenderingBackend::getApplicationName() {
	return mApplicationName;
}

uint16_t Atlas::VulkanRenderingBackend::initInstance(const APIVersion& cAPIVersionRef, bool cbEnableValidationLayers, std::string appName)
{
	vkb::InstanceBuilder instanceBuilder;

	auto instanceReturn = instanceBuilder.set_app_name("Example Vulkan Application")
		.request_validation_layers(cbEnableValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(cAPIVersionRef.major, cAPIVersionRef.minor, cAPIVersionRef.patch)
		.build();

	vkb::Instance vulkanInstance = instanceReturn.value();

	this->mVulkanInstance = vulkanInstance.instance;
	this->mVulkanDebugMessenger = vulkanInstance.debug_messenger;
	return 0;
}

void Atlas::VulkanRenderingBackend::createSwapchain(uint32_t width, uint32_t height)
{
	vkb::SwapchainBuilder swapchainBuilder{ mVulkanGPUDevice, mVulkanDevice, mVulkanSurface };

	mVulkanSwapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = mVulkanSwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

	mVulkanSwapchainExtent = vkbSwapchain.extent;
	//store swapchain and its related images
	mVulkanSwapchain = vkbSwapchain.swapchain;
	mVulkanSwapchainImages = vkbSwapchain.get_images().value();
	mVulkanSwapchainImageViews = vkbSwapchain.get_image_views().value();
}

void Atlas::VulkanRenderingBackend::destroySwapchain()
{
	vkDestroySwapchainKHR(mVulkanDevice, mVulkanSwapchain, nullptr);

	// destroy swapchain resources
	for (int i = 0; i < mVulkanSwapchainImageViews.size(); i++) {

		vkDestroyImageView(mVulkanDevice, mVulkanSwapchainImageViews[i], nullptr);
	}
}

void Atlas::VulkanRenderingBackend::init(AGameWindow* windowHandle)
{
	//ATLAS_ASSERT(!windowHandle->isInitialized() && !windowHandle->isOpen(), "WINDOW MUST BE OPENED PRIOR TO VULKAN INIT!");

	if (windowHandle == nullptr || !windowHandle->isInitialized() || !windowHandle->isOpen()) {
		std::cout << "Window must be set up and open prior to Vulkan init" << std::endl;
	}

	const bool cbEnableValidationLayers = isErrorCheckingEnabled();
	const APIVersion cApiVersion = getAPIVersion();

	vkb::InstanceBuilder instanceBuilder;

	// TODO: Figure out how to stop OBS from overriding the API version
	auto instanceReturn = instanceBuilder.set_app_name("Example Vulkan Application")
		.request_validation_layers(cbEnableValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(cApiVersion.major, cApiVersion.minor, cApiVersion.patch)
		.build();

	vkb::Instance vulkanBootstrapInstance = instanceReturn.value();

	this->mVulkanInstance = vulkanBootstrapInstance.instance;
	this->mVulkanDebugMessenger = vulkanBootstrapInstance.debug_messenger;

	// Device init

#ifdef ATLAS_USE_SDL2
	SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(windowHandle->getUncastWindowHandle()), mVulkanInstance, &mVulkanSurface);
#endif

	//vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features.dynamicRendering = true;
	features.synchronization2 = true;

	//vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;
	
	//use vkbootstrap to select a gpu. 
	//We want a gpu that can write to the SDL surface and supports vulkan 1.3 with the correct features
	vkb::PhysicalDeviceSelector selector{ vulkanBootstrapInstance };
	selector.set_minimum_version(1, 3);
	selector.set_required_features_13(features);
	selector.set_required_features_12(features12);
	selector.set_surface(mVulkanSurface);

	vkb::PhysicalDevice physicalDevice = selector.select().value();


	//create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	vkb::Device vkbDevice = deviceBuilder.build().value();

	// Get the VkDevice handle used in the rest of a vulkan application
	mVulkanDevice = vkbDevice.device;
	mVulkanGPUDevice = physicalDevice.physical_device;
	
	auto activeWindowDescription = std::make_unique<WindowDescription>(windowHandle->getWindowDescription());

	createSwapchain(activeWindowDescription->windowRectangle.width, activeWindowDescription->windowRectangle.height);

	mIsInitialized = true;
}

void Atlas::VulkanRenderingBackend::update()
{

}

void Atlas::VulkanRenderingBackend::shutdown()
{
	//vkDestroyInstance(this->mVulkanInstance, nullptr);

	if (mIsInitialized) {
		destroySwapchain();

		vkDestroySurfaceKHR(mVulkanInstance, mVulkanSurface, nullptr);
		vkDestroyDevice(mVulkanDevice, nullptr);

		vkb::destroy_debug_utils_messenger(mVulkanInstance, mVulkanDebugMessenger);
		vkDestroyInstance(mVulkanInstance, nullptr);
	}
	
}

bool Atlas::VulkanRenderingBackend::checkValidationLayerSupport() {
	//uint32_t layerCount;
	//vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	//std::vector<VkLayerProperties> availableLayers(layerCount);
	//vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


	//for (const char* layerName : this->mValidationLayers) {
	//	bool layerFound = false;

	//	for (const auto& layerProperties : availableLayers) {
	//		if (strcmp(layerName, layerProperties.layerName) == 0) {
	//			layerFound = true;
	//			break;
	//		}
	//	}

	//	if (!layerFound) {
	//		return false;
	//	}
	//}

	return true;
}

#endif

