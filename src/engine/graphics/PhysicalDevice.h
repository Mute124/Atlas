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

	class PhysicalDevice {
	private:
		VkPhysicalDevice mPhysicalDeviceHandle = VK_NULL_HANDLE;
		std::unique_ptr<vkb::PhysicalDevice> mVkbDevicePtr = nullptr;

		friend class VulkanRenderingBackend;

	public:

		uint32_t init(VulkanInstanceWrapper& cVulkanInstanceRef, PhysicalDeviceFeaturesAggregate const& cPhysicalDeviceFeatures) {
			vkb::PhysicalDeviceSelector selector{ cVulkanInstanceRef.getVulkanBootstrapInstance() };

		}

		VkPhysicalDevice& getHandle();

		vkb::PhysicalDevice& getVkbDevice()
		{
			return *mVkbDevicePtr;
		}
	};

	class Device {
	private:
		VkDevice mDeviceHandle = VK_NULL_HANDLE;

		friend class VulkanRenderingBackend;
	public:
	};
}
