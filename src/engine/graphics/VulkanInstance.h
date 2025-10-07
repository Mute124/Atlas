/**************************************************************************************************
 * @file Instance.h
 * 
 * @brief The wrapper for the vulkan instance.
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <memory>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <VkBootstrap.h>

#include "../core/Core.h"
#include "../core/Version.h"

#include "backend/RenderingBackend.h"

#ifdef ATLAS_DEBUG
	#define ATLAS_ENABLE_VALIDATION_LAYERS true
#endif


namespace Atlas {
	using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	class VulkanInstance {
	private:

		static inline const std::string_view scEngineName = "Atlas";

		std::unique_ptr<vkb::Instance> mVulkanBootstrapInstance;

		VkInstance mVulkanInstance = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;

		std::optional<VulkanDebugCallback> mDebugMessengerCallback;
		std::string mApplicationName;

		Version mVulkanVersion;

		bool mbEnableValidationLayers;

		friend class VulkanRenderingBackend;
	public:
		VulkanInstance();

		~VulkanInstance();

		void setVersion(const Version& cVulkanVersionRef);

		void setApplicationName(std::string_view appName);

		void setEnableValidationLayers(const bool cbEnableValidationLayers);

		void setDebugCallback(VulkanDebugCallback debugMessengerCallback);

		uint16_t init();

		void shutdown();

		VkInstance& getInstance();

		VkDebugUtilsMessengerEXT& getDebugMessenger();

		vkb::Instance& getVulkanBootstrapInstance();

		explicit operator const VkInstance&() const;
	};
}
