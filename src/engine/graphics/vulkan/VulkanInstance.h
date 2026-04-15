/**************************************************************************************************
 * @file VulkanInstance.h
 * 
 * @brief The wrapper for the vulkan instance.
 * 
 * @date September 2025
 * 
 * @since v0.0.1
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License. 
 * ***************************************************************************************************/
#pragma once
#include <vector>
#include <cstdint>
#include <optional>
#include <string>
#include <memory>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <VkBootstrap.h>

#include "../../core/Core.h"
#include "../../core/Version.h"
#include "../../core/ApplicationInfo.h"

#include "../backend/RenderingBackend.h"

#ifdef ATLAS_DEBUG
	#define ATLAS_ENABLE_VALIDATION_LAYERS true

#else
	#define ATLAS_ENABLE_VALIDATION_LAYERS true
#endif

namespace Atlas {
	using VulkanDebugCallback = PFN_vkDebugUtilsMessengerCallbackEXT;

	/**
	 * @brief Acts as a wrapper for VkInstance and it's debug messenger. Furthermore, this class handles it's own initialization and shutdown.
	 * Please see the note below for an importance notice.
	 * 
	 * @note Due to some weird voodoo stuff going on with Vulkan, this class must be initialized using the @ref init() function instead of
	 * the constructor, as in the case of other classes.
	 * 
	 * @since v0.0.1
	 */
	class VulkanInstanceWrapper {
	public:
		struct InitConfiguration {
			ApplicationInfo applicationInfo;

			std::string engineName{ "Atlas" };
			Version atlasVersion{ ATLAS_VERSION_MAJOR, ATLAS_VERSION_MINOR, ATLAS_VERSION_PATCH };

			bool bEnableValidationLayers{ ATLAS_ENABLE_VALIDATION_LAYERS };

			std::vector<const char*> requestedExtensions{};
			std::vector<const char*> requestedValidationLayers{ "VK_LAYER_KHRONOS_validation" };

			std::optional<VulkanDebugCallback> optionalDebugMessengerCallback;

			InitConfiguration(const ApplicationInfo& applicationInfo, const std::string& engineName, const Version& atlasVersion, bool bEnableValidationLayers, const std::optional<VulkanDebugCallback>& optionalDebugMessengerCallback)
				: applicationInfo(applicationInfo), engineName(engineName), atlasVersion(atlasVersion), bEnableValidationLayers(bEnableValidationLayers), optionalDebugMessengerCallback(optionalDebugMessengerCallback) {}

			InitConfiguration(const ApplicationInfo& applicationInfo, const std::string& engineName, const Version& atlasVersion, bool bEnableValidationLayers)
				: InitConfiguration(applicationInfo, engineName, atlasVersion, bEnableValidationLayers, std::nullopt) {}

			InitConfiguration()
				: InitConfiguration(ApplicationInfo{}, "Atlas", { ATLAS_VERSION_MAJOR, ATLAS_VERSION_MINOR, ATLAS_VERSION_PATCH }, ATLAS_ENABLE_VALIDATION_LAYERS, std::nullopt) {}
		};
	private:
		friend class VulkanRenderingBackend;

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

		Version mVulkanVersion;

		bool mbEnableValidationLayers{ ATLAS_ENABLE_VALIDATION_LAYERS };

		bool mbIsInitialized{ false };

		static bool IsSuccessfulEnumerationResult(VkResult enumerationResult);

		/**
		 * @brief Creates a @c vkb::Instance and returns it after configuring a @c vkb::InstanceBuilder. 
		 * 
		 * @return A @c vkb::Result of type @a vkb::Instance. This is what is returned by
		 * @c vkb::InstanceBuilder::build(). 
		 * 
		 * @since v0.0.1
		 */
		vkb::Result<vkb::Instance> createVulkanInstance();

		/**
		 * @brief Sets the application name and engine name of the configuration info of the
		 * @c vkb::InstanceBuilder instance passed.
		 * 
		 * @param instanceBuilder A @c vkb::InstanceBuilder instance.
		 * 
		 * @since v0.0.1
		 */
		void setupNameConfigurations(vkb::InstanceBuilder& instanceBuilder);

		/**
		 * @brief Sets the engine version and required API version of the configuration info of the
		 * @c vkb::InstanceBuilder instance passed.
		 * 
		 * @param instanceBuilder A @c vkb::InstanceBuilder instance.
		 * 
		 * @since v0.0.1
		 */
		void setupVersions(vkb::InstanceBuilder& instanceBuilder);

		/**
		 * @brief Enables the validation layers requested in the @c InitConfiguration of this class by iterating
		 * over the requested validation layers, checking if they are supported and then enabling them if they are.
		 * If a requested validation layer is not supported, an exception is thrown.
		 * 
		 * @param instanceBuilder The instance builder to enable the validation layers in
		 * 
		 * @since v0.0.1
		 */
		void enableValidationLayers(vkb::InstanceBuilder& instanceBuilder);

		/**
		 * @brief Sets the debug callback for the Vulkan instance builder based on the optional debug messenger callback
		 * in the initialization configuration. If the optional debug messenger callback is present in the initialization
		 * configuration, it is set as the debug callback. Otherwise, if the @c ATLAS_USE_VULKAN_DEFAULT_DEBUG_MESSENGER
		 * macro is defined, the default debug messenger is used.  Otherwise, the @c DefaultVulkanDebugCallback function
		 * is used as the debug callback.
		 *
		 * @param instanceBuilder The Vulkan instance builder to set the debug callback for.
		 * 
		 * @since v0.0.1
		 */
		void setupDebugMessenger(vkb::InstanceBuilder& instanceBuilder);

	public:

		explicit VulkanInstanceWrapper(InitConfiguration initConfiguration);

		VulkanInstanceWrapper() = default;

		/**
		 * @brief Destroys the VulkanInstanceWrapper instance by calling the shutdown function.
		 * 
		 * @since v0.0.1
		 */
		~VulkanInstanceWrapper();

		/**
		 * @brief Retrieves the number of available Vulkan instance layers.
		 * 
		 * @return The number of available Vulkan instance layers.
		 * 
		 * @since v0.0.1
		 */
		static uint32_t GetAvailableLayerCount();

		/**
		 * @brief Retrieves the available Vulkan instance layers and returns them in a vector.
		 * 
		 * @return A @c std::vector of @a VkLayerProperties containing the available Vulkan instance layers.
		 * 
		 * @since v0.0.1
		 */
		static std::vector<VkLayerProperties> GetAvailableLayers();

		/**
		 * @brief Checks if a Vulkan instance layer is supported.
		 * 
		 * @param layerName The name of the Vulkan instance layer to check support for.
		 * @param availableLayers A @c std::vector of @a VkLayerProperties containing all of the available Vulkan instance layers.
		 * 
		 * @return @c true if the Vulkan instance layer is supported, @c false otherwise.
		 * 
		 * @since v0.0.1
		 */
		static bool IsLayerSupported(const char* layerName, std::vector<VkLayerProperties> const& availableLayers);

		/**
		 * @brief Initializes the @c VulkanInstanceWrapper instance. 
		 * 
		 * @pre You must first call the constructor before calling this function and give it an initialization configuration.
		 * 
		 * @since v0.0.1
		 */
		void init();

		/**
		 * @brief Shuts everything down and cleans it up.
		 * 
		 * @note This should be done automatically when this class goes out of scope as it is called in the destructor.
		 * 
		 * @since v0.0.1
		 */
		void shutdown();

		/**
		 * @brief Retrieves the VkInstance handle.
		 * 
		 * @return The current VkInstance handle.
		 * 
		 * @since v0.0.1
		 */
		VkInstance& getInstance();

		/**
		 * @brief Retrieves the VkDebugUtilsMessengerEXT handle.
		 * 
		 * @return The current VkDebugUtilsMessengerEXT handle.
		 * 
		 * @since v0.0.1
		 */
		VkDebugUtilsMessengerEXT& getDebugMessenger();

		/**
		 * @brief Retrieves the @c vkb::Instance handle that was created by the VulkanBootstrap library.
		 * 
		 * @pre @ref init() must be called before this function is called, otherwise it will return an empty @c vkb::Instance.
		 * 
		 * @return The @c vkb::Instance handle created by the VulkanBootstrap library on initialization.
		 * 
		 * @since v0.0.1
		 */
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
