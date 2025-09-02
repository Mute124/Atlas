/**************************************************************************************************
 * @file VKDevice.cpp
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "VKDevice.h"
#include "RenderingBackend.h"
#include "../Frame.h"
#include "../../core/Core.h"
#include "../window/Window.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
//#include "vk_mem_alloc.h"


#include <cstdint>
#include <iostream>
#include <algorithm>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <span>

// This avoids the transitive include of string_view on MSVC compilers
#ifdef ATLAS_COMPILER_MSVC
	#include <__msvc_string_view.hpp>
#endif // ATLAS_COMPILER_MSVC

#ifdef ATLAS_USE_SDL2
	#include <SDL2/SDL_video.h>
#endif // ATLAS_USE_SDL2

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>


	#include <VkBootstrap.h>

	#ifdef ATLAS_USE_SDL2
		#include <SDL_vulkan.h>
	#endif // ATLAS_USE_SDL2


#endif // ATLAS_USE_VULKAN



#ifdef ATLAS_USE_VULKAN

Atlas::VulkanRenderingBackend* gLoadedVulkanBackend = nullptr;

void Atlas::DescriptorLayoutBuilder::add_binding(uint32_t binding, VkDescriptorType type)
{
	VkDescriptorSetLayoutBinding newbind{};
	newbind.binding = binding;
	newbind.descriptorCount = 1;
	newbind.descriptorType = type;

	bindings.push_back(newbind);
}

void Atlas::DescriptorLayoutBuilder::clear()
{
	bindings.clear();
}

VkDescriptorSetLayout Atlas::DescriptorLayoutBuilder::build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext, VkDescriptorSetLayoutCreateFlags flags)
{
	for (auto& b : bindings) {
		b.stageFlags |= shaderStages;
	}

	VkDescriptorSetLayoutCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	info.pNext = pNext;

	info.pBindings = bindings.data();
	info.bindingCount = (uint32_t)bindings.size();
	info.flags = flags;

	VkDescriptorSetLayout set;
	vkCreateDescriptorSetLayout(device, &info, nullptr, &set);

	return set;
}


void Atlas::DescriptorAllocator::init_pool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios)
{
	std::vector<VkDescriptorPoolSize> poolSizes;
	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = uint32_t(ratio.ratio * maxSets)
		});
	}

	VkDescriptorPoolCreateInfo pool_info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	pool_info.flags = 0;
	pool_info.maxSets = maxSets;
	pool_info.poolSizeCount = (uint32_t)poolSizes.size();
	pool_info.pPoolSizes = poolSizes.data();

	vkCreateDescriptorPool(device, &pool_info, nullptr, &pool);
}

void Atlas::DescriptorAllocator::clear_descriptors(VkDevice device)
{
	vkResetDescriptorPool(device, pool, 0);
}

void Atlas::DescriptorAllocator::destroy_pool(VkDevice device)
{
	vkDestroyDescriptorPool(device, pool, nullptr);
}

VkDescriptorSet Atlas::DescriptorAllocator::allocate(VkDevice device, VkDescriptorSetLayout layout)
{
	VkDescriptorSetAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkDescriptorSet ds;
	vkAllocateDescriptorSets(device, &allocInfo, &ds);

	return ds;
}

void Atlas::VulkanRenderingBackend::setApplicationName(std::string_view applicationName) {
	mApplicationName = applicationName;
}

std::string Atlas::VulkanRenderingBackend::getApplicationName() {
	return mApplicationName;
}

void Atlas::VulkanRenderingBackend::setFenceTimeout(uint64_t lengthInNS) {
	mFencesTimeoutNS = lengthInNS;
}

void Atlas::VulkanRenderingBackend::setNextImageTimeout(uint64_t lengthInNS) {
	mNextImageTimeoutNS = lengthInNS;
}

Atlas::FrameData& Atlas::VulkanRenderingBackend::getCurrentFrame()
{
	return mFrameDataArray[mCurrentFrameNumber % FRAME_OVERLAP];
}

uint16_t Atlas::VulkanRenderingBackend::initInstance(const APIVersion& cAPIVersionRef, bool cbEnableValidationLayers, std::string const& appName)
{
	vkb::InstanceBuilder instanceBuilder;

	auto instanceReturn = instanceBuilder.set_app_name(appName.c_str())
		.request_validation_layers(cbEnableValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(cAPIVersionRef.major, cAPIVersionRef.minor, cAPIVersionRef.patch)
		.build();

	vkb::Instance vulkanInstance = instanceReturn.value();
	
	this->mVulkanInstance = vulkanInstance.instance;
	this->mDebugMessenger = vulkanInstance.debug_messenger;

	return 0;
}

void Atlas::VulkanRenderingBackend::initDescriptors()
{
	//create a descriptor pool that will hold 10 sets with 1 image each
	std::vector<DescriptorAllocator::PoolSizeRatio> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 }
	};

	mGlobalDescriptorAllocator.init_pool(mDevice, 10, sizes);

	//make the descriptor set layout for our compute draw
	{
		DescriptorLayoutBuilder builder;
		builder.add_binding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
		mDrawImageDescriptorLayout = builder.build(mDevice, VK_SHADER_STAGE_COMPUTE_BIT);
	}

	// other code
	//allocate a descriptor set for our draw image
	mDrawImageDescriptors = mGlobalDescriptorAllocator.allocate(mDevice, mDrawImageDescriptorLayout);

	VkDescriptorImageInfo imgInfo{};
	imgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	imgInfo.imageView = mDrawImage.imageView;

	VkWriteDescriptorSet drawImageWrite = {};
	drawImageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	drawImageWrite.pNext = nullptr;

	drawImageWrite.dstBinding = 0;
	drawImageWrite.dstSet = mDrawImageDescriptors;
	drawImageWrite.descriptorCount = 1;
	drawImageWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	drawImageWrite.pImageInfo = &imgInfo;

	vkUpdateDescriptorSets(mDevice, 1, &drawImageWrite, 0, nullptr);

	//make sure both the descriptor allocator and the new layout get cleaned up properly
	mMainDeletionQueue.push_function([&]() {
		mGlobalDescriptorAllocator.destroy_pool(mDevice);

		vkDestroyDescriptorSetLayout(mDevice, mDrawImageDescriptorLayout, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::initPipelines()
{
	initBackgroundPipelines();


}

void Atlas::VulkanRenderingBackend::initBackgroundPipelines()
{
	VkPipelineLayoutCreateInfo computeLayout{};
	computeLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	computeLayout.pNext = nullptr;
	computeLayout.pSetLayouts = &mDrawImageDescriptorLayout;
	computeLayout.setLayoutCount = 1;

	vkCreatePipelineLayout(mDevice, &computeLayout, nullptr, &mGradientPipelineLayout);
	
	//layout code
	VkShaderModule computeDrawShader;
	if (!LoadShaderModule("C:/Dev/Techstorm-v5/shaders/gradient.comp.spv", mDevice, &computeDrawShader))
	{
		std::cout << "Error when building the compute shader \n";
	}
	
	VkPipelineShaderStageCreateInfo stageinfo{};
	stageinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stageinfo.pNext = nullptr;
	stageinfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	stageinfo.module = computeDrawShader;
	stageinfo.pName = "main";

	VkComputePipelineCreateInfo computePipelineCreateInfo{};
	computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	computePipelineCreateInfo.pNext = nullptr;
	computePipelineCreateInfo.layout = mGradientPipelineLayout;
	computePipelineCreateInfo.stage = stageinfo;

	vkCreateComputePipelines(mDevice, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &mGradientPipeline);

	vkDestroyShaderModule(mDevice, computeDrawShader, nullptr);

	mMainDeletionQueue.push_function([&]() {
		vkDestroyPipelineLayout(mDevice, mGradientPipelineLayout, nullptr);
		vkDestroyPipeline(mDevice, mGradientPipeline, nullptr);
	});
}

void Atlas::VulkanRenderingBackend::initCommands()
{
	//create a command pool for commands submitted to the graphics queue.
	//we also want the pool to allow for resetting of individual command buffers
	VkCommandPoolCreateInfo commandPoolInfo = CreateCommandPoolCreateInfo(mGraphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	for (int i = 0; i < FRAME_OVERLAP; i++) {

		vkCreateCommandPool(mDevice, &commandPoolInfo, nullptr, &mFrameDataArray.at(i).commandPool);

		// allocate the default command buffer that we will use for rendering
		VkCommandBufferAllocateInfo cmdAllocInfo = CreateCommandBufferAllocateInfo(mFrameDataArray.at(i).commandPool, 1);

		vkAllocateCommandBuffers(mDevice, &cmdAllocInfo, &mFrameDataArray.at(i).mainCommandBuffer);
	}

	//create syncronization structures
	//one fence to control when the gpu has finished rendering the frame,
	//and 2 semaphores to syncronize rendering with swapchain
	//we want the fence to start signalled so we can wait on it on the first frame
	VkFenceCreateInfo fenceCreateInfo = CreateFenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
	VkSemaphoreCreateInfo semaphoreCreateInfo = CreateSemaphoreCreateInfo();

	for (int i = 0; i < FRAME_OVERLAP; i++) {
		vkCreateFence(mDevice, &fenceCreateInfo, nullptr, &mFrameDataArray.at(i).renderFence);

		vkCreateSemaphore(mDevice, &semaphoreCreateInfo, nullptr, &mFrameDataArray.at(i).swapchainSemaphore);
		vkCreateSemaphore(mDevice, &semaphoreCreateInfo, nullptr, &mFrameDataArray.at(i).renderSemaphore);
	}

}

void Atlas::VulkanRenderingBackend::createSwapchain(uint32_t width, uint32_t height)
{
	vkb::SwapchainBuilder swapchainBuilder{ mGPUDevice, mDevice, mSurface };
	
	mSwapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = mSwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

	mSwapchainExtent = vkbSwapchain.extent;
	//store swapchain and its related images
	mSwapchain = vkbSwapchain.swapchain;
	mSwapchainImages = vkbSwapchain.get_images().value();
	mSwapchainImageViews = vkbSwapchain.get_image_views().value();
}

void Atlas::VulkanRenderingBackend::destroySwapchain()
{
	vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);

	// destroy swapchain resources
	for (int i = 0; i < mSwapchainImageViews.size(); i++) {

		vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
	}
}

void Atlas::VulkanRenderingBackend::init(AGameWindow* gameWindow)
{
	// Just make sure everything is in order before Vulkan gets initialized
	if (gameWindow == nullptr || canInitialize(gameWindow) == false) {
		std::cout << "Window must be set up and open prior to Vulkan init" << std::endl;
		return;
	}

	const bool cbEnableValidationLayers = isErrorCheckingEnabled();
	const APIVersion cApiVersion = getAPIVersion();

	vkb::InstanceBuilder instanceBuilder;
	instanceBuilder.set_app_name(mApplicationName.c_str());
	instanceBuilder.request_validation_layers(cbEnableValidationLayers);
	instanceBuilder.use_default_debug_messenger(); // TODO: allow the user to set what messenger to use
	instanceBuilder.require_api_version(cApiVersion.major, cApiVersion.minor, cApiVersion.patch);

	// TODO: Figure out how to stop OBS from overriding the API version.
	// The issue is that, if OBS is installed, it will override the API 
	// version, even if a higher version is set. This is a problem because
	// it can cause the API version to be set to a lower version than what
	// we want. Currently, it is not a major problem, but it is something
	// to keep in mind.
	auto instanceReturn = instanceBuilder.build();
	
	vkb::Instance vulkanBootstrapInstance = instanceReturn.value();

	this->mVulkanInstance = vulkanBootstrapInstance.instance;
	this->mDebugMessenger = vulkanBootstrapInstance.debug_messenger;

	// Device init
	
	// If vulkan is being used with SDL2, this has to be done here.
#ifdef ATLAS_USE_SDL2
	// directly passing the window cast to avoid memory issues.
	SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(gameWindow->getUncastWindowHandle()), mVulkanInstance, &mSurface);
#endif // ATLAS_USE_SDL2
	
	// Vulkan 1.3 features
	//--------------------
	constexpr bool cbEnableDynamicRendering = true;
	constexpr bool cbEnableSynchronization2 = true;

	VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features.dynamicRendering = cbEnableDynamicRendering;
	features.synchronization2 = cbEnableSynchronization2;

	// Vulkan 1.2 features
	//--------------------

	constexpr bool cbEnableBufferDeviceAddress = true;
	constexpr bool cbEnableDescriptorIndexing = true;

	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = cbEnableBufferDeviceAddress;
	features12.descriptorIndexing = cbEnableDescriptorIndexing;
	
	// use vkbootstrap to select a gpu. 
	//We want a gpu that can write to the SDL surface and supports vulkan 1.3 with the correct features
	vkb::PhysicalDeviceSelector selector{ vulkanBootstrapInstance };
	selector.set_minimum_version(cApiVersion.major, cApiVersion.minor);
	selector.set_required_features_13(features);
	selector.set_required_features_12(features12);
	selector.set_surface(mSurface);

	vkb::PhysicalDevice physicalDevice = selector.select().value();

	//create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	vkb::Device vkbDevice = deviceBuilder.build().value();

	// Get the VkDevice handle used in the rest of a vulkan application
	mDevice = vkbDevice.device;
	mGPUDevice = physicalDevice.physical_device;
	
	mGraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	mGraphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

	// initialize the memory allocator (this can be a function)
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = mGPUDevice;
	allocatorInfo.device = mDevice;
	allocatorInfo.instance = mVulkanInstance;
	allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	vmaCreateAllocator(&allocatorInfo, &mVMAAllocator);

	mMainDeletionQueue.push_function([&]() {
		vmaDestroyAllocator(mVMAAllocator);
	});

	initSwapchain(gameWindow);

	initCommands();

	initDescriptors();

	initPipelines();

	mIsInitialized = true;
}

void Atlas::VulkanRenderingBackend::initSwapchain(AGameWindow* gameWindow)
{
	// swapchain init

	const WindowDescription activeWindowDescription = gameWindow->getWindowDescription();

	createSwapchain(activeWindowDescription.windowRectangle.width, activeWindowDescription.windowRectangle.height);

	//draw image size will match the window
	VkExtent3D drawImageExtent = {
		activeWindowDescription.windowRectangle.width,
		activeWindowDescription.windowRectangle.height,
		1
	};

	//hardcoding the draw format to 32 bit float
	mDrawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
	mDrawImage.imageExtent = drawImageExtent;

	VkImageUsageFlags drawImageUsages{};
	drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VkImageCreateInfo rimg_info = CreateImageCreateInfo(mDrawImage.imageFormat, drawImageUsages, drawImageExtent);

	//for the draw image, we want to allocate it from gpu local memory
	VmaAllocationCreateInfo rimg_allocinfo = {};
	rimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	rimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	//allocate and create the image
	vmaCreateImage(mVMAAllocator, &rimg_info, &rimg_allocinfo, &mDrawImage.image, &mDrawImage.allocation, nullptr);

	//build a image-view for the draw image to use for rendering
	VkImageViewCreateInfo rview_info = CreateImageViewCreateInfo(mDrawImage.imageFormat, mDrawImage.image, VK_IMAGE_ASPECT_COLOR_BIT);

	vkCreateImageView(mDevice, &rview_info, nullptr, &mDrawImage.imageView);

	//add to deletion queues
	mMainDeletionQueue.push_function([=]() {
		vkDestroyImageView(mDevice, mDrawImage.imageView, nullptr);
		vmaDestroyImage(mVMAAllocator, mDrawImage.image, mDrawImage.allocation);
	});
}

void Atlas::VulkanRenderingBackend::update()
{

}

void Atlas::VulkanRenderingBackend::resetFences(const uint32_t cFenceCount, FrameData& currentFrame)
{
	vkWaitForFences(mDevice, cFenceCount, &currentFrame.renderFence, true, mFencesTimeoutNS);

	currentFrame.deletionQueue.flush();

	vkResetFences(mDevice, cFenceCount, &currentFrame.renderFence);
}

void Atlas::VulkanRenderingBackend::draw()
{
	const static uint32_t cFenceCount = 1;

	FrameData& currentFrame = getCurrentFrame();

	resetFences(cFenceCount, currentFrame);

	//request image from the swapchain
	uint32_t swapchainImageIndex;
	vkAcquireNextImageKHR(mDevice, mSwapchain, mNextImageTimeoutNS, currentFrame.swapchainSemaphore, nullptr, &swapchainImageIndex);

	// reset command buffer
	VkCommandBufferResetFlags cmdResetFlags = 0;

	//naming it cmd for shorter writing
	VkCommandBuffer cmd = currentFrame.mainCommandBuffer;

	// now that we are sure that the commands finished executing, we can safely
	// reset the command buffer to begin recording again.
	vkResetCommandBuffer(cmd, cmdResetFlags);

	//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
	VkCommandBufferBeginInfo cmdBeginInfo = CreateCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	mDrawExtent.width = mDrawImage.imageExtent.width;
	mDrawExtent.height = mDrawImage.imageExtent.height;

	vkBeginCommandBuffer(cmd, &cmdBeginInfo);

	// transition our main draw image into general layout so we can write into it
	// we will overwrite it all so we dont care about what was the older layout
	TransitionImage(cmd, mDrawImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

	drawBackground(cmd);

	//transition the draw image and the swapchain image into their correct transfer layouts
	TransitionImage(cmd, mDrawImage.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	TransitionImage(cmd, mSwapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// execute a copy from the draw image into the swapchain
	CopyImageToImage(cmd, mDrawImage.image, mSwapchainImages[swapchainImageIndex], mDrawExtent, mSwapchainExtent);

	// set swapchain image layout to Present so we can show it on the screen
	TransitionImage(cmd, mSwapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	//finalize the command buffer (we can no longer add commands, but it can now be executed)
	vkEndCommandBuffer(cmd);

	//prepare the submission to the queue. 
	//we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
	//we will signal the _renderSemaphore, to signal that rendering has finished

	VkCommandBufferSubmitInfo cmdinfo = SubmitCommandBufferInfo(cmd);

	VkSemaphoreSubmitInfo waitInfo = SubmitSemaphoreInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, currentFrame.swapchainSemaphore);
	VkSemaphoreSubmitInfo signalInfo = SubmitSemaphoreInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, currentFrame.renderSemaphore);

	VkSubmitInfo2 submit = SubmitInfo(&cmdinfo, &signalInfo, &waitInfo);

	//submit command buffer to the queue and execute it.
	// _renderFence will now block until the graphic commands finish execution
	vkQueueSubmit2(mGraphicsQueue, 1, &submit, currentFrame.renderFence);

	// prepare present
	// this will put the image we just rendered to into the visible window.
	// we want to wait on the _renderSemaphore for that, 
	// as its necessary that drawing commands have finished before the image is displayed to the user
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.pSwapchains = &mSwapchain;
	presentInfo.swapchainCount = 1;

	presentInfo.pWaitSemaphores = &currentFrame.renderSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	presentInfo.pImageIndices = &swapchainImageIndex;

	vkQueuePresentKHR(mGraphicsQueue, &presentInfo);

	//increase the number of frames drawn
	mCurrentFrameNumber++;
}

void Atlas::VulkanRenderingBackend::drawBackground(VkCommandBuffer cmd)
{
	//make a clear-color from frame number. This will flash with a 120 frame period.
	VkClearColorValue clearValue;
	float flash = std::abs(std::sin(mCurrentFrameNumber / 120.f));
	clearValue = { { 0.0f, 0.0f, flash, 1.0f } };

	VkImageSubresourceRange clearRange = CreateImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);

	//clear image
	vkCmdClearColorImage(cmd, mDrawImage.image, VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);

	// bind the gradient drawing compute pipeline
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, mGradientPipeline);

	// bind the descriptor set containing the draw image for the compute pipeline
	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, mGradientPipelineLayout, 0, 1, &mDrawImageDescriptors, 0, nullptr);

	// execute the compute pipeline dispatch. We are using 16x16 workgroup size so we need to divide by it
	vkCmdDispatch(cmd, std::ceil(mDrawExtent.width / 16.0), std::ceil(mDrawExtent.height / 16.0), 1);
}

void Atlas::VulkanRenderingBackend::present()
{
}

void Atlas::VulkanRenderingBackend::shutdown()
{

	// No need to worry about cleaning up if not initialized, hence the check
	if (mIsInitialized) {

		vkDeviceWaitIdle(mDevice);

		for (int i = 0; i < FRAME_OVERLAP; i++) {
			vkDestroyCommandPool(mDevice, mFrameDataArray.at(i).commandPool, nullptr);

			//destroy sync objects
			vkDestroyFence(mDevice, mFrameDataArray.at(i).renderFence, nullptr);
			vkDestroySemaphore(mDevice, mFrameDataArray.at(i).renderSemaphore, nullptr);
			vkDestroySemaphore(mDevice, mFrameDataArray.at(i).swapchainSemaphore, nullptr);

			mFrameDataArray.at(i).deletionQueue.flush();
		}

		mMainDeletionQueue.flush();

		destroySwapchain();

		vkDestroySurfaceKHR(mVulkanInstance, mSurface, nullptr);
		vkDestroyDevice(mDevice, nullptr);

		vkb::destroy_debug_utils_messenger(mVulkanInstance, mDebugMessenger);
		vkDestroyInstance(mVulkanInstance, nullptr);

		mIsInitialized = false;
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

bool Atlas::VulkanRenderingBackend::canInitialize(AGameWindow* gameWindow)
{
	return gameWindow->isInitialized() && gameWindow->isOpen();
}

VkCommandPoolCreateInfo Atlas::CreateCommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
	VkCommandPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.queueFamilyIndex = queueFamilyIndex;
	info.flags = flags;
	return info;
}

VkCommandBufferAllocateInfo Atlas::CreateCommandBufferAllocateInfo(VkCommandPool pool, uint32_t count)
{
	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;

	info.commandPool = pool;
	info.commandBufferCount = count;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	return info;
}

VkFenceCreateInfo Atlas::CreateFenceCreateInfo(VkFenceCreateFlags flags)
{
	VkFenceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;

	info.flags = flags;

	return info;
}

VkSemaphoreCreateInfo Atlas::CreateSemaphoreCreateInfo(VkSemaphoreCreateFlags flags)
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	return info;
}

VkSemaphoreSubmitInfo Atlas::SubmitSemaphoreInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore)
{
	VkSemaphoreSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.semaphore = semaphore;
	submitInfo.stageMask = stageMask;
	submitInfo.deviceIndex = 0;
	submitInfo.value = 1;

	return submitInfo;
}

VkCommandBufferSubmitInfo Atlas::SubmitCommandBufferInfo(VkCommandBuffer cmd)
{
	VkCommandBufferSubmitInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
	info.pNext = nullptr;
	info.commandBuffer = cmd;
	info.deviceMask = 0;

	return info;
}

VkSubmitInfo2 Atlas::SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo, VkSemaphoreSubmitInfo* waitSemaphoreInfo)
{
	VkSubmitInfo2 info = {};
	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
	info.pNext = nullptr;

	info.waitSemaphoreInfoCount = waitSemaphoreInfo == nullptr ? 0 : 1;
	info.pWaitSemaphoreInfos = waitSemaphoreInfo;

	info.signalSemaphoreInfoCount = signalSemaphoreInfo == nullptr ? 0 : 1;
	info.pSignalSemaphoreInfos = signalSemaphoreInfo;

	info.commandBufferInfoCount = 1;
	info.pCommandBufferInfos = cmd;

	return info;
}

VkCommandBufferBeginInfo Atlas::CreateCommandBufferBeginInfo(VkCommandBufferUsageFlags flags)
{
	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.pNext = nullptr;

	info.pInheritanceInfo = nullptr;
	info.flags = flags;
	return info;
}

void Atlas::TransitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout)
{
	VkImageMemoryBarrier2 imageBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
	imageBarrier.pNext = nullptr;

	imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
	imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

	imageBarrier.oldLayout = currentLayout;
	imageBarrier.newLayout = newLayout;

	VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	imageBarrier.subresourceRange = CreateImageSubresourceRange(aspectMask);
	imageBarrier.image = image;

	VkDependencyInfo depInfo{};
	depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
	depInfo.pNext = nullptr;

	depInfo.imageMemoryBarrierCount = 1;
	depInfo.pImageMemoryBarriers = &imageBarrier;

	vkCmdPipelineBarrier2(cmd, &depInfo);
}

VkImageSubresourceRange Atlas::CreateImageSubresourceRange(VkImageAspectFlags aspectMask)
{
	VkImageSubresourceRange subImage{};
	subImage.aspectMask = aspectMask;
	subImage.baseMipLevel = 0;
	subImage.levelCount = VK_REMAINING_MIP_LEVELS;
	subImage.baseArrayLayer = 0;
	subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

	return subImage;
}

VkImageCreateInfo Atlas::CreateImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent)
{
	VkImageCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	info.pNext = nullptr;

	info.imageType = VK_IMAGE_TYPE_2D;

	info.format = format;
	info.extent = extent;

	info.mipLevels = 1;
	info.arrayLayers = 1;

	//for MSAA. we will not be using it by default, so default it to 1 sample per pixel.
	info.samples = VK_SAMPLE_COUNT_1_BIT;

	//optimal tiling, which means the image is stored on the best gpu format
	info.tiling = VK_IMAGE_TILING_OPTIMAL;
	info.usage = usageFlags;

	return info;
}

VkImageViewCreateInfo Atlas::CreateImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags)
{
	// build a image-view for the depth image to use for rendering
	VkImageViewCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	info.pNext = nullptr;

	info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	info.image = image;
	info.format = format;
	info.subresourceRange.baseMipLevel = 0;
	info.subresourceRange.levelCount = 1;
	info.subresourceRange.baseArrayLayer = 0;
	info.subresourceRange.layerCount = 1;
	info.subresourceRange.aspectMask = aspectFlags;

	return info;
}

void Atlas::CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize)
{
	VkImageBlit2 blitRegion{ .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr };

	blitRegion.srcOffsets[1].x = srcSize.width;
	blitRegion.srcOffsets[1].y = srcSize.height;
	blitRegion.srcOffsets[1].z = 1;

	blitRegion.dstOffsets[1].x = dstSize.width;
	blitRegion.dstOffsets[1].y = dstSize.height;
	blitRegion.dstOffsets[1].z = 1;

	blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blitRegion.srcSubresource.baseArrayLayer = 0;
	blitRegion.srcSubresource.layerCount = 1;
	blitRegion.srcSubresource.mipLevel = 0;

	blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blitRegion.dstSubresource.baseArrayLayer = 0;
	blitRegion.dstSubresource.layerCount = 1;
	blitRegion.dstSubresource.mipLevel = 0;

	VkBlitImageInfo2 blitInfo{ .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
	blitInfo.dstImage = destination;
	blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	blitInfo.srcImage = source;
	blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	blitInfo.filter = VK_FILTER_LINEAR;
	blitInfo.regionCount = 1;
	blitInfo.pRegions = &blitRegion;

	vkCmdBlitImage2(cmd, &blitInfo);
}

bool Atlas::LoadShaderModule(const char* filePath, VkDevice device, VkShaderModule* outShaderModule)
{
	// open the file. With cursor at the end
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		return false;
	}

	// find what the size of the file is by looking up the location of the cursor
	// because the cursor is at the end, it gives the size directly in bytes
	size_t fileSize = (size_t)file.tellg();

	// spirv expects the buffer to be on uint32, so make sure to reserve a int
	// vector big enough for the entire file
	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

	// put file cursor at beginning
	file.seekg(0);

	// load the entire file into the buffer
	file.read((char*)buffer.data(), fileSize);

	// now that the file is loaded into the buffer, we can close it
	file.close();

	// create a new shader module, using the buffer we loaded
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;

	// codeSize has to be in bytes, so multply the ints in the buffer by size of
	// int to know the real size of the buffer
	createInfo.codeSize = buffer.size() * sizeof(uint32_t);
	createInfo.pCode = buffer.data();

	// check that the creation goes well.
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		return false;
	}
	*outShaderModule = shaderModule;
	return true;
}

Atlas::VulkanRenderingBackend& Atlas::getLoadedRenderingBacked()
{
	return *gLoadedVulkanBackend;
}

void Atlas::setLoadedRenderingBackend(VulkanRenderingBackend* backend)
{
	if (gLoadedVulkanBackend != nullptr) {
		gLoadedVulkanBackend->shutdown();

		delete gLoadedVulkanBackend;
	}

	gLoadedVulkanBackend = backend;
}

void Atlas::resetLoadedRenderingBackend()
{
	setLoadedRenderingBackend(nullptr);
}

#endif

void Atlas::Pipeline::createLayout(VkPipelineLayoutCreateInfo layoutInfo, VkDevice* device)
{
	vkCreatePipelineLayout(*device, &layoutInfo, nullptr, &mPipelineLayout);
}

void Atlas::Pipeline::createLayout(VkDevice* device, VkDescriptorSetLayout* descriptorSetLayout) {
	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.pNext = nullptr;
	layoutInfo.pSetLayouts = descriptorSetLayout;
	layoutInfo.setLayoutCount = 1;

	createLayout(layoutInfo, device);
}

Atlas::Shader::Shader(std::filesystem::path compiledShaderPath, std::string name) : mCompiledShaderPath(compiledShaderPath), mName(name) {}

void Atlas::Shader::destroyModule(VkDevice* device) {
	vkDestroyShaderModule(*device, mShaderModule, nullptr);
}

bool Atlas::Shader::load(VkDevice* device) {
	return LoadShaderModule(mCompiledShaderPath.string().c_str(), *device, &mShaderModule);
}

VkShaderModule Atlas::Shader::getModule() const { return mShaderModule; }
