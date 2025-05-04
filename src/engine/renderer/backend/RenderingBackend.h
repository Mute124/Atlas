/**************************************************************************************************
 * @file RenderingBackend.h
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <string>
#include <functional>

#include "../../core/Core.h"

#include "../../debugging/Debugger.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif
#include <cstdint>
#include <vector>

namespace Atlas {
	class ARenderingBackend {
	protected:
		const std::string mAPIName;

	public:

		virtual void init() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;


	};

#ifdef ATLAS_USE_VULKAN
	
	class VulkanRenderingBackend : public ARenderingBackend {
	private:

		VkInstance mVulkanInstance = VK_NULL_HANDLE;
		std::vector<const char*> mValidationLayers;

	protected:

		VkApplicationInfo mAppInfo;
		VkInstanceCreateInfo mCreateInfo;

	public:

		VulkanRenderingBackend(const VulkanRenderingBackend&) = delete;

		/**
		 * @brief Default constructor.
		 *
		 * @note This constructor is not intended to be used because the @ref mAppInfo and @ref mCreateInfo are
		 * required to create a Vulkan instance (and they are declared as a const!). Additionally, the variable
		 * @ref mCreateInfo requires information on extensions and such, and since Atlas does not mandate any
		 * specific window management system (ie. GLFW), the @ref mAppInfo @b MUST be set prior to calling this.
		 *
		 * @since v0.0.1
		 *
		 * @sa @ref mAppInfo
		 * @sa @ref mCreateInfo
		 */
		VulkanRenderingBackend() = default;

		void init() override;

		void update() override;

		void shutdown() override;

		bool checkValidationLayerSupport();
	};
#endif
}