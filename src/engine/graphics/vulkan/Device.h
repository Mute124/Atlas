/**************************************************************************************************
 * @file Device.h
 * 
 * @brief Contains declarations that would be used by the device.
 * 
 * @date March 2026
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <VkBootstrap.h>

#include "../RenderCommon.h"

namespace Atlas {
	//class Device
	//	: public VulkanGlobalStateObject<VkDevice, Device>
	//	//: public AVulkanHandleWrapper<VkDevice/*, vkb::Device, vkb::DeviceBuilder*/> 
	//{
	//private:

	//	friend class VulkanRenderingBackend;

	//public:
	//	vkb::Device mVkbDevice;

	//	//using AVulkanHandleWrapper<VkDevice/*, vkb::Device, vkb::DeviceBuilder*/>::AVulkanHandleWrapper;
	//	using VulkanGlobalStateObject<VkDevice, Device>::VulkanGlobalStateObject;

	//	Device() = default;

	//	~Device() override;

	//	void waitIdle();
	//};
}
