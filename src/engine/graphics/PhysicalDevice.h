/**************************************************************************************************
 * @file PhysicalDevice.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <VkBootstrap.h>

#include "VulkanInstance.h"

namespace Atlas {
	struct PhysicalDeviceFeaturesAggregate {
		VkPhysicalDeviceVulkan13Features vulkan13Features;
		VkPhysicalDeviceVulkan12Features vulkan12Features;
	};

	struct PhysicalDeviceSelectionConstraints {
		PhysicalDeviceFeaturesAggregate physicalDeviceFeatures;
		Version minimumAPIVersion;
		Version preferredAPIVersion;
	};


	class PhysicalDevice {
	private:
		VkPhysicalDevice mPhysicalDeviceHandle = VK_NULL_HANDLE;

		VkPhysicalDeviceProperties mDeviceProperties;
		VkPhysicalDeviceFeatures mDeviceFeatures;
		VkPhysicalDeviceMemoryProperties mDeviceMemoryProperties;

		std::unique_ptr<vkb::PhysicalDeviceSelector> mVkbDeviceSelectorPtr = nullptr;
		std::unique_ptr<vkb::PhysicalDevice> mVkbDevicePtr = nullptr;

		/*PhysicalDeviceFeaturesAggregate mPhysicalDeviceFeaturesAggregate;

		Version mMinimumApiVersion;
		Version mPreferredApiVersion;*/

		PhysicalDeviceSelectionConstraints mSelectionConstraints;

		friend class VulkanRenderingBackend;

		void populateDeviceProperties();

	public:

		explicit PhysicalDevice(PhysicalDeviceSelectionConstraints const& selectionConstraints);

		PhysicalDevice() = default;

		vkb::PhysicalDevice init(VulkanInstanceWrapper& cVulkanInstanceRef, VkSurfaceKHR surface);

		vkb::PhysicalDeviceSelector selectDevice(VulkanInstanceWrapper& cVulkanInstanceRef, VkSurfaceKHR surface);

		VkPhysicalDevice& getHandle();

		vkb::PhysicalDevice& getVkbDevice();
	};

	class Device {
	private:
		VkDevice mDeviceHandle = VK_NULL_HANDLE;

		friend class VulkanRenderingBackend;
	public:
	};
}
