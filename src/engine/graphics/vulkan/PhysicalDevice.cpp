/**************************************************************************************************
 * @file PhysicalDevice.cpp
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <memory>

#include <VkBootstrap.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../RenderCommon.h"
#include "PhysicalDevice.h"
#include "../../debugging/Logging.h"
#include "VulkanInstance.h"
#include "../../core/Common.h"



Atlas::PhysicalDeviceProperties::PhysicalDeviceProperties(VkPhysicalDevice physicalDevice)
{
	// This is calling out of date functions!
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &features);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
}

Atlas::PhysicalDeviceProperties::PhysicalDeviceProperties(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features, const VkPhysicalDeviceMemoryProperties& memoryProperties)
	: properties(properties), features(features), memoryProperties(memoryProperties)
{
}



// conversion operator to VkPhysicalDevice
vkb::PreferredDeviceType Atlas::ToVkbPreferredDeviceType(EPhysicalDeviceType preferredDeviceType) {
	switch(preferredDeviceType) {
		
		// These statements just reduces verbosity
		using enum Atlas::EPhysicalDeviceType;
		using enum vkb::PreferredDeviceType;

		case Other:
			return other;

		case Integrated:
			return integrated;

		case Discrete:
			return discrete;

		case Virtual_GPU:
			return virtual_gpu;

		case CPU:
			return cpu;

		default:
			return discrete;
	}
}

std::shared_ptr<Atlas::Device> Atlas::GetMainVulkanDevice() {
	return Device::GetMainHandle();
}

Atlas::Device::~Device()
{
}

void Atlas::Device::waitIdle()
{
	vkDeviceWaitIdle(getHandle());
}
