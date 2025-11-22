/**************************************************************************************************
 * @file Instance.h
 * 
 * @brief The wrapper for the vulkan instance.
 * 
 * @date September 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <memory>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <VkBootstrap.h>

#include "../../core/Core.h"
#include "../../core/Version.h"

#include "../backend/RenderingBackend.h"

#ifdef ATLAS_DEBUG
	#define ATLAS_ENABLE_VALIDATION_LAYERS true
#endif

namespace Atlas {
	using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	/**
	 * @brief Acts as a wrapper for VkInstance and it's debug messenger. Furthermore, this class handles it's own initialization and shutdown.
	 * 
	 * @since v0.0.1
	 */
	class VulkanInstanceWrapper {
	public:
		struct InitConfiguration {
			std::string applicationName{ "test" };
			std::string engineName{ "Atlas" };

			Version vulkanVersion{1, 3, 0};
			Version atlasVersion{ ATLAS_VERSION_MAJOR, ATLAS_VERSION_MINOR, ATLAS_VERSION_PATCH };

			bool bEnableValidationLayers{ ATLAS_ENABLE_VALIDATION_LAYERS };

			std::optional<VulkanDebugCallback> optionalDebugMessengerCallback;
		};
	private:
		friend class VulkanRenderingBackend;

		//static inline const std::string_view scEngineName{ "Atlas" };

		vkb::Instance mVulkanBootstrapInstance;
		
		/**
		 * @brief According to the Khronos specification, @a VkInstance is an opaque handle to a Vulkan instance. By default, this is set to
		 * @a VK_NULL_HANDLE.
		 * 
		 * @sa @link https://registry.khronos.org/vulkan/specs/latest/man/html/VkInstance.html @endlink
		 * 
		 * @since v0.0.1
		 */
		VkInstance mVulkanInstance{ VK_NULL_HANDLE };

		VkDebugUtilsMessengerEXT mDebugMessenger{ VK_NULL_HANDLE };

		InitConfiguration mInitConfiguration;

		//std::optional<VulkanDebugCallback> mDebugMessengerCallback;
		//std::string mApplicationName;

		Version mVulkanVersion;

		bool mbEnableValidationLayers{ ATLAS_ENABLE_VALIDATION_LAYERS };

		static vkb::InstanceBuilder CreateInstanceBuilder(InitConfiguration const& initConfiguration);
	public:
		explicit VulkanInstanceWrapper(vkb::InstanceBuilder instanceBuilder);

		explicit VulkanInstanceWrapper(InitConfiguration const& initConfiguration = InitConfiguration{});

		//VulkanInstanceWrapper() = default;

		~VulkanInstanceWrapper();

		//void setVersion(const Version& cVulkanVersionRef);

		//void setApplicationName(std::string_view appName);

		//void setEnableValidationLayers(const bool cbEnableValidationLayers);

		//void setDebugCallback(VulkanDebugCallback debugMessengerCallback);

		uint16_t init();

		void shutdown();

		VkInstance& getInstance();

		VkDebugUtilsMessengerEXT& getDebugMessenger();

		vkb::Instance& getVulkanBootstrapInstance();

		explicit operator const VkInstance&() const;
	};

	/**
	 * @brief Translates a vulkan debug log level to one that Atlas can use. For the most part, the Vulkan debug log levels are the same as the
	 * Atlas ones, with @a VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, @a VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT, and
	 * @a VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT being the same as @a ELogLevel::info, @a ELogLevel::warn, and @a ELogLevel::err in that order.
	 * However, @a VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT and @a VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT are translated to 
	 * @a ELogLevel::trace and @a ELogLevel::critical, respectively. The default is @a ELogLevel::info.
	 * 
	 * @param messageSeverity The Vulkan debug log level to translate.
	 * 
	 * @returns The translated log level. See the description for more information.
	 * 
	 * @since v0.0.1
	 */
	ELogLevel TranslateVulkanLogLevel(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);

	/**
	 * @brief The default Vulkan debug callback that Atlas uses if no custom debug callback is provided by the user in VulkanInstanceWrapper before it
	 * is initialized. According to the Khronos specification on the @a PFN_vkDebugUtilsMessengerCallbackEXT object, this function should always return 
	 * a value of @a VK_FALSE, which is what this function does. See the Khronos specification for more information (this is in the see also section of
	 * the documentation). The message is logged via the usage of @a spdlog::log(). However, this means that some logger (more specifically a @a Spdlog
	 * logger) must have been initialized before this function is called or it will crash. 
	 * 
	 * @remark While most of these parameters are not used, they are included for consistency with the specifications that are listed in the Khronos
	 * specifications at the link in the <b>see also</b> section.
	 * 
	 * @pre A Spdlog logger must have been initialized before this function is called. This should be handled by the @ref SpdlogLogger class.
	 * 
	 * @param messageSeverity The Vulkan debug log level reported by the vulkan debug callback. This is translated to an Atlas log level using
	 * the TranslateVulkanLogLevel function.
	 * 
	 * @param messageType The type of message reported by the vulkan debug callback.
	 * 
	 * @param pCallbackData A pointer to the vulkan debug callback data. Within this parameter, the message is located, which is what is passed to be
	 * logged via @a spdlog::log().
	 * 
	 * @param pUserData A pointer to user data that was provided when the vulkan debug callback was set.
	 * 
	 * @returns Always returns @a VK_FALSE.
	 * 
	 * @sa @link https://registry.khronos.org/vulkan/specs/latest/man/html/PFN_vkDebugUtilsMessengerCallbackEXT.html @endlink
	 * 
	 * @todo Instead of logging via @a spdlog::log(), perhaps consider writing another function that will log using an already initialized Atlas logger.
	 * As of right now, this is assuming that the user is using the @ref SpdlogLogger class, which is the default. However, the user should be able to
	 * use another logger and still have this function work. 
	 * 
	 * @since v0.0.1
	 */
	[[nodiscard]] VkBool32 DefaultVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
}
