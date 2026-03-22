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

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <VkBootstrap.h>

#include "VulkanInstance.h"
#include "../../core/Version.h"
#include "../RenderCommon.h"

#include "../../debugging/Logging.h"
#include "../../debugging/AException.h"
#include <string>


namespace Atlas {
	using GPUExtension = const char*;

	enum class EPhysicalDeviceType {
		Other = 0,
		Integrated = 1,
		Discrete = 2,
		Virtual_GPU = 3,
		CPU = 4
	};

	//struct GPUExtension {
	//	GPUExtensionName name;
	//	bool bRequired{ false };
	//};

	struct PhysicalDeviceFeaturesAggregate {
		VkPhysicalDeviceVulkan13Features vulkan13Features;
		VkPhysicalDeviceVulkan12Features vulkan12Features;
	};

	struct PhysicalDeviceSelectionConstraints {
		PhysicalDeviceFeaturesAggregate physicalDeviceFeatures;
		std::vector<GPUExtension> requiredDeviceExtensions;

		Version minimumAPIVersion;
		Version preferredAPIVersion;

		std::optional<std::string> preferredDeviceName;
		EPhysicalDeviceType preferredDeviceType{ EPhysicalDeviceType::Discrete };
		VkDeviceSize requiredDeviceMemorySize{ 0 };

		VkSurfaceKHR surface{ VK_NULL_HANDLE };

		bool bAllowAnyDeviceType{ true };
		bool bAlwaysSelectFirstDevice{ false };

		//bool bAlwaysSelectBestDevice{ false };
		
		bool bRequireDedicatedComputeQueue{ false };
		bool bRequireDedicatedTransferQueue{ false };

		bool bRequireSeparateComputeQueue{ false };
		bool bRequireSeparateTransferQueue{ false };

		bool bRequirePresent{true};

		bool bDisablePortabilitySubset{ false };

		bool bDeferDeviceSelection{ false };
		bool bDeferSurfaceInit{ false };

		PhysicalDeviceSelectionConstraints() = default;
	};

	struct PhysicalDeviceProperties {
		VkPhysicalDeviceProperties properties{};
		VkPhysicalDeviceFeatures features{};
		VkPhysicalDeviceMemoryProperties memoryProperties{};

		PhysicalDeviceProperties(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceFeatures& features, const VkPhysicalDeviceMemoryProperties& memoryProperties);
		explicit PhysicalDeviceProperties(VkPhysicalDevice physicalDevice);
		PhysicalDeviceProperties() = default;
	};

	class PhysicalDevice {
	public:
		using PhysicalDeviceSelector = vkb::PhysicalDeviceSelector;
	private:
		
		PhysicalDeviceProperties mDevicePropertiesAggregate{};
		vkb::PhysicalDevice mVkbDevice;
		
	protected:

		void setVkbHandle(vkb::PhysicalDevice const& physicalDevice) {
			mVkbDevice = physicalDevice;
		}
	public:

		void init(PhysicalDeviceSelector const& physicalDeviceSelector) {
			InfoLog("Attempting to initialize PhysicalDevice");

			if (physicalDeviceSelector.select_device_names().value().empty() == true) {
				throw AException("PhysicalDeviceSelector must have at least one device name that can be selected. This can be caused by failing to initialize the device selector.");
			}

			const std::string cSelectedDeviceName = physicalDeviceSelector.select_device_names().value().at(0);

			InfoLog(std::format("Selected {} as the physical device.", cSelectedDeviceName));

			setVkbHandle(physicalDeviceSelector.select().value());

			if (!isValid()) {
				throw AException("Failed to initialize PhysicalDevice because the physical device and/or the VkPhysicalDevice handle was null.");
			}

			populateDeviceProperties();

			InfoLog(std::format("{} has been successfully initialized.", cSelectedDeviceName));
		}

		void init(VulkanInstanceWrapper& cVulkanInstanceRef) {

		}
		
		void populateDeviceProperties() {
			if (!hasValidVkbHandle() || !hasValidVkHandle()) {
				throw AException("Failed to populate PhysicalDeviceProperties because the physical device and/or the VkPhysicalDevice handle was null or invalid.");
			}

			mDevicePropertiesAggregate = PhysicalDeviceProperties(getVkHandle());
		}

		PhysicalDeviceSelector selectDevice(VulkanInstanceWrapper& cVulkanInstanceRef, PhysicalDeviceSelectionConstraints const& selectionConstraints);

		bool hasValidVkbHandle() const {
			return getVkbHandle() != nullptr;
		}

		bool hasValidVkHandle() const {
			return getVkHandle() != VK_NULL_HANDLE;
		}

		bool isValid() const {
			return hasValidVkbHandle() && hasValidVkHandle();
		}

		vkb::PhysicalDevice const& getVkbHandle() const {
			return mVkbDevice;
		}

		VkPhysicalDevice getVkHandle() const {
			return getVkbHandle().physical_device;
		}

		std::string getName() const {
			return getVkbHandle().properties.deviceName;
		}
	};

	class Device 
		: public VulkanGlobalStateObject<VkDevice, Device>
		//: public AVulkanHandleWrapper<VkDevice/*, vkb::Device, vkb::DeviceBuilder*/> 
	{
	private:

		friend class VulkanRenderingBackend;
		
	public:
		vkb::Device mVkbDevice;

		//using AVulkanHandleWrapper<VkDevice/*, vkb::Device, vkb::DeviceBuilder*/>::AVulkanHandleWrapper;
		using VulkanGlobalStateObject<VkDevice, Device>::VulkanGlobalStateObject;

		Device() = default;

		~Device() override;
		
		void waitIdle();
	};

	vkb::PreferredDeviceType ToVkbPreferredDeviceType(EPhysicalDeviceType preferredDeviceType);
	
	std::shared_ptr<Device> GetMainVulkanDevice();

}
