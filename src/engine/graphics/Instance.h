/**************************************************************************************************
 * @file Instance.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <optional>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <VkBootstrap.h>

#include "../core/Core.h"

#include "backend/RenderingBackend.h"

#ifdef ATLAS_DEBUG
	#define ATLAS_ENABLE_VALIDATION_LAYERS true
#endif

namespace Atlas {
	using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	class Instance {
	private:
		VkInstance mVulkanInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		
		friend class VulkanRenderingBackend;
	public:

		void init(const bool cbEnableValidationLayers, const Version& cVulkanVersionRef, std::string const& appName, std::optional<VulkanDebugCallback> debugCallback) {
			vkb::InstanceBuilder instanceBuilder;

			instanceBuilder.set_app_name(appName.c_str());
			instanceBuilder.set_engine_name("Atlas");

			instanceBuilder.request_validation_layers(cbEnableValidationLayers);

			if (debugCallback.has_value()) {
				instanceBuilder.set_debug_callback(debugCallback.value());
			}
			else {
				instanceBuilder.use_default_debug_messenger();
			}

			instanceBuilder.require_api_version(cVulkanVersionRef.major, cVulkanVersionRef.minor, cVulkanVersionRef.patch);

			auto instanceReturn = instanceBuilder.build();
			vkb::Instance vulkanBootstrapInstance = instanceReturn.value();
			mVulkanInstance = vulkanBootstrapInstance.instance;
			mDebugMessenger = vulkanBootstrapInstance.debug_messenger;
		}

		VkInstance getInstance() const { 
			return mVulkanInstance;
		}

		VkDebugUtilsMessengerEXT getDebugMessenger() const { 
			return mDebugMessenger; 
		}

	};
}
