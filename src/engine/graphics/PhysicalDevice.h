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

	class PhysicalDevice : public Validatable {
	private:
		//VkPhysicalDeviceProperties mDeviceProperties;
		//VkPhysicalDeviceFeatures mDeviceFeatures;
		//VkPhysicalDeviceMemoryProperties mDeviceMemoryProperties;

		std::unique_ptr<vkb::PhysicalDeviceSelector> mVkbDeviceSelectorPtr{ nullptr };
		vkb::PhysicalDevice mVkbDevice;

		std::string mDeviceName;

		//bool mbIsValid{ false };

		/*PhysicalDeviceFeaturesAggregate mPhysicalDeviceFeaturesAggregate;

		Version mMinimumApiVersion;
		Version mPreferredApiVersion;*/
		VkPhysicalDevice mPhysicalDeviceHandle{ VK_NULL_HANDLE };

		PhysicalDeviceProperties mDevicePropertiesAggregate{};

		PhysicalDeviceSelectionConstraints mSelectionConstraints;

		friend class VulkanRenderingBackend;

	protected:
		void populateDeviceProperties();

		void evaluateValidity();

		//void setValidity(bool bNewValue) override;

		//void setInvalid();

		//void setValid() noexcept;

		void setPhysicalDeviceHandle(VkPhysicalDevice const& handle);

		void setDeviceName(std::string_view deviceName);
	public:
		explicit PhysicalDevice(VulkanInstanceWrapper& cVulkanInstanceRef, PhysicalDeviceSelectionConstraints const& selectionConstraints);

		PhysicalDevice() = default;

		//vkb::PhysicalDevice init(VulkanInstanceWrapper& cVulkanInstanceRef);

		vkb::PhysicalDeviceSelector selectDevice(VulkanInstanceWrapper& cVulkanInstanceRef);
		
		bool isValid() const final;

		PhysicalDeviceProperties getPhysicalDeviceProperties() const;

		VkPhysicalDevice getHandle();

		vkb::PhysicalDevice& getVkbDevice();

		// Returns a string_view so that the string cannot be modified
		std::string_view getName() const;

		// conversion operator to VkPhysicalDevice
		explicit(false) operator const VkPhysicalDevice& () const;
		
		// Conversion operator to bool (true if valid)
		explicit(false) operator bool() const { return isValid(); }
	};

	class Device {
	private:
		VkDevice mDeviceHandle = VK_NULL_HANDLE;

		friend class VulkanRenderingBackend;
	public:
	};

	vkb::PreferredDeviceType ToVkbPreferredDeviceType(EPhysicalDeviceType preferredDeviceType);
}
