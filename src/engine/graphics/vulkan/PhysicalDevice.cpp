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

Atlas::PhysicalDevice::PhysicalDeviceSelector Atlas::PhysicalDevice::selectDevice(VulkanInstanceWrapper & cVulkanInstanceRef, PhysicalDeviceSelectionConstraints const& selectionConstraints) {
	vkb::PhysicalDeviceSelector selector{ cVulkanInstanceRef.getVulkanBootstrapInstance() };

	if (selectionConstraints.preferredDeviceName.has_value()) {
		selector.set_name(selectionConstraints.preferredDeviceName.value());
	}
	else {
		selector.set_minimum_version(selectionConstraints.minimumAPIVersion.majorVersion, selectionConstraints.minimumAPIVersion.minorVersion);
		selector.set_required_features_13(selectionConstraints.physicalDeviceFeatures.vulkan13Features);
		selector.set_required_features_12(selectionConstraints.physicalDeviceFeatures.vulkan12Features);
		selector.allow_any_gpu_device_type(selectionConstraints.bAllowAnyDeviceType);

		if (selectionConstraints.bDeferSurfaceInit) {
			selector.defer_surface_initialization();
		}

		if (selectionConstraints.bDisablePortabilitySubset) {
			selector.disable_portability_subset();
		}

		if (selectionConstraints.bRequireDedicatedComputeQueue) {
			selector.require_dedicated_compute_queue();
		}

		if (selectionConstraints.bRequireDedicatedTransferQueue) {
			selector.require_dedicated_transfer_queue();
		}

		if (selectionConstraints.bRequireSeparateComputeQueue) {
			selector.require_separate_compute_queue();
		}

		if (selectionConstraints.bRequireSeparateTransferQueue) {
			selector.require_separate_transfer_queue();
		}

		selector.prefer_gpu_device_type(ToVkbPreferredDeviceType(selectionConstraints.preferredDeviceType));
		selector.required_device_memory_size(selectionConstraints.requiredDeviceMemorySize);

		selector.require_present(selectionConstraints.bRequirePresent);
		selector.select_first_device_unconditionally(selectionConstraints.bAlwaysSelectFirstDevice);
	}

	selector.add_required_extensions(selectionConstraints.requiredDeviceExtensions);
	selector.set_surface(selectionConstraints.surface);

	return selector;
}
