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

VkPhysicalDevice& Atlas::PhysicalDevice::getHandle() {
	return mPhysicalDeviceHandle; 
}

