/**************************************************************************************************
 * @file PhysicalDevice.cpp
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <VkBootstrap.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "PhysicalDevice.h"
#include "../debugging/Logging.h"

void Atlas::PhysicalDevice::populateDeviceProperties()
{
	vkGetPhysicalDeviceProperties(mPhysicalDeviceHandle, &mDeviceProperties);
	vkGetPhysicalDeviceFeatures(mPhysicalDeviceHandle, &mDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(mPhysicalDeviceHandle, &mDeviceMemoryProperties);
}

Atlas::PhysicalDevice::PhysicalDevice(PhysicalDeviceSelectionConstraints const& selectionConstraints)
	: mSelectionConstraints(selectionConstraints)
{
}

vkb::PhysicalDevice Atlas::PhysicalDevice::init(VulkanInstanceWrapper& cVulkanInstanceRef, VkSurfaceKHR surface) {
	vkb::PhysicalDeviceSelector selector = selectDevice(cVulkanInstanceRef, surface);
	vkb::PhysicalDevice vkbPhysicalDevice = selector.select().value();
	
	populateDeviceProperties();
	
	return vkbPhysicalDevice;
}

vkb::PhysicalDeviceSelector Atlas::PhysicalDevice::selectDevice(VulkanInstanceWrapper& cVulkanInstanceRef, VkSurfaceKHR surface)
{
	vkb::PhysicalDeviceSelector selector{cVulkanInstanceRef.getVulkanBootstrapInstance()};

	selector.set_minimum_version(mSelectionConstraints.minimumAPIVersion.major, mSelectionConstraints.minimumAPIVersion.minor);
	selector.set_required_features_13(mSelectionConstraints.physicalDeviceFeatures.vulkan13Features);
	selector.set_required_features_12(mSelectionConstraints.physicalDeviceFeatures.vulkan12Features);
	selector.set_surface(surface);
	

	return selector;
}

VkPhysicalDevice& Atlas::PhysicalDevice::getHandle() {
	return mPhysicalDeviceHandle; 
}

vkb::PhysicalDevice& Atlas::PhysicalDevice::getVkbDevice()
{
	return *mVkbDevicePtr;
}

