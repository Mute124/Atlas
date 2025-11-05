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

#include "RenderCommon.h"
#include "PhysicalDevice.h"
#include "../debugging/Logging.h"
#include "VulkanInstance.h"
#include "../core/Common.h"



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

void Atlas::PhysicalDevice::populateDeviceProperties()
{
	VkPhysicalDevice vkHandle = getHandle();
	mDevicePropertiesAggregate = PhysicalDeviceProperties(vkHandle);
}

void Atlas::PhysicalDevice::evaluateValidity()
{
	// Since the handle stored within this class is a copy of the handle stored in vkb::PhysicalDevice,
	// we need to make sure that both handles are valid
	if (!this->isValid()) {
		
		this->setInvalid();
	}
}

//void Atlas::PhysicalDevice::setValidity(bool bNewValue) noexcept {
//	mbIsValid = bNewValue;
//}
//
//void Atlas::PhysicalDevice::setInvalid() noexcept {
//	setValidity(false);
//}
//
//void Atlas::PhysicalDevice::setValid() noexcept {
//	setValidity(true);
//}

void Atlas::PhysicalDevice::setDeviceName(std::string_view deviceName) { 
	mDeviceName = deviceName;
}

Atlas::PhysicalDevice::PhysicalDevice(VulkanInstanceWrapper& cVulkanInstanceRef, PhysicalDeviceSelectionConstraints const& selectionConstraints)
	: AVulkanCompositeHandleWrapper(), mSelectionConstraints(selectionConstraints)
{
	vkb::PhysicalDeviceSelector selector = selectDevice(cVulkanInstanceRef);
	//mVkbDevice = selector.select().value();
	setVkbHandle(selector.select().value());

	// by default, vulkan bootstrap will set the handle to VK_NULL_HANDLE on initialization. 
	// If it is not set to VK_NULL_HANDLE, then the device is valid.
	//if (IsInvalidVulkanHandle<VkPhysicalDevice>(mVkbDevice.physical_device)) {
	//	setInvalid();
	//	return;
	//}

	//mPhysicalDeviceHandle = mVkbDevice.physical_device;
	setHandle(getVkbHandle().physical_device);
	setDeviceName(getVkbHandle().name);
	
	this->evaluateValidity();

	populateDeviceProperties();
}

//vkb::PhysicalDevice Atlas::PhysicalDevice::init(VulkanInstanceWrapper& cVulkanInstanceRef) {
//	vkb::PhysicalDeviceSelector selector = selectDevice(cVulkanInstanceRef);
//	vkb::PhysicalDevice vkbPhysicalDevice = selector.select().value();
//	
//	populateDeviceProperties();
//	
//	return vkbPhysicalDevice;
//}

vkb::PhysicalDeviceSelector Atlas::PhysicalDevice::selectDevice(VulkanInstanceWrapper& cVulkanInstanceRef)
{
	vkb::PhysicalDeviceSelector selector{cVulkanInstanceRef.getVulkanBootstrapInstance()};

	if(mSelectionConstraints.preferredDeviceName.has_value()) {

		selector.set_name(mSelectionConstraints.preferredDeviceName.value());
		
	} else {
		
		selector.set_minimum_version(mSelectionConstraints.minimumAPIVersion.major, mSelectionConstraints.minimumAPIVersion.minor);
		selector.set_required_features_13(mSelectionConstraints.physicalDeviceFeatures.vulkan13Features);
		selector.set_required_features_12(mSelectionConstraints.physicalDeviceFeatures.vulkan12Features);
		selector.allow_any_gpu_device_type(mSelectionConstraints.bAllowAnyDeviceType);
		
		if (mSelectionConstraints.bDeferSurfaceInit) {
			selector.defer_surface_initialization();
		}

		if (mSelectionConstraints.bDisablePortabilitySubset) {
			selector.disable_portability_subset();
		}

		if (mSelectionConstraints.bRequireDedicatedComputeQueue) {
			selector.require_dedicated_compute_queue();
		}
		
		if (mSelectionConstraints.bRequireDedicatedTransferQueue) {
			selector.require_dedicated_transfer_queue();
		}

		if (mSelectionConstraints.bRequireSeparateComputeQueue) {
			selector.require_separate_compute_queue();
		}

		if (mSelectionConstraints.bRequireSeparateTransferQueue) {
			selector.require_separate_transfer_queue();
		}

		selector.prefer_gpu_device_type(ToVkbPreferredDeviceType(mSelectionConstraints.preferredDeviceType));
		selector.required_device_memory_size(mSelectionConstraints.requiredDeviceMemorySize);
	
		selector.require_present(mSelectionConstraints.bRequirePresent);
		selector.select_first_device_unconditionally(mSelectionConstraints.bAlwaysSelectFirstDevice);
	}

	selector.add_required_extensions(mSelectionConstraints.requiredDeviceExtensions);
	selector.set_surface(mSelectionConstraints.surface);

	return selector;
}

bool Atlas::PhysicalDevice::isValid() const {
	//const bool cbParentResult = Validatable::isValid();
	//const bool cbHandleValid = IsValidVulkanHandle<VkPhysicalDevice>(mPhysicalDeviceHandle)
	//	&& IsValidVulkanHandle<VkPhysicalDevice>(mVkbDevice.physical_device);
	
	return AVulkanHandleWrapper::isValid();
}

Atlas::PhysicalDeviceProperties Atlas::PhysicalDevice::getPhysicalDeviceProperties() const
{
	return mDevicePropertiesAggregate;
}

//vkb::PhysicalDevice& Atlas::PhysicalDevice::getVkbHandle()
//{
//	return mVkbDevice;
//}

std::string_view Atlas::PhysicalDevice::getName() const {
	return mDeviceName;
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

Atlas::Device::~Device()
{
}
