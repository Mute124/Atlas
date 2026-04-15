/**************************************************************************************************
 * @file VulkanInstance.cpp
 * 
 * @brief .
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
#include <optional>
#include <cstdint>
#include <memory>
#include <string>
#include <algorithm>

#include "VulkanInstance.h"
#include "../../core/Core.h"
#include "../../core/Version.h"
#include "../../debugging/Logging.h"
#include "../../debugging/AException.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

inline bool Atlas::VulkanInstanceWrapper::IsSuccessfulEnumerationResult(VkResult enumerationResult)
{
	// According to vkEnumerateInstanceLayerProperties documentation, if enumerationResult is VK_SUCCESS or VK_INCOMPLETE, then the enumeration was successful
	return enumerationResult == VK_SUCCESS || enumerationResult == VK_INCOMPLETE;
}

vkb::Result<vkb::Instance> Atlas::VulkanInstanceWrapper::createVulkanInstance()
{
	vkb::InstanceBuilder instanceBuilder;

	setupNameConfigurations(instanceBuilder);
	setupVersions(instanceBuilder);

	const bool cbRequestValidationLayers = mInitConfiguration.bEnableValidationLayers;
	instanceBuilder.request_validation_layers(cbRequestValidationLayers);
	
	if (cbRequestValidationLayers) {
		enableValidationLayers(instanceBuilder);
	}

	setupDebugMessenger(instanceBuilder);

	return instanceBuilder.build();
}

void Atlas::VulkanInstanceWrapper::setupNameConfigurations(vkb::InstanceBuilder& instanceBuilder)
{
	instanceBuilder.set_app_name(mInitConfiguration.applicationInfo.name.c_str());
	instanceBuilder.set_engine_name(mInitConfiguration.engineName.c_str());
}

void Atlas::VulkanInstanceWrapper::setupVersions(vkb::InstanceBuilder& instanceBuilder)
{
	instanceBuilder.set_engine_version(mInitConfiguration.atlasVersion.majorVersion, mInitConfiguration.atlasVersion.minorVersion, mInitConfiguration.atlasVersion.patchVersion);
	instanceBuilder.require_api_version(mInitConfiguration.applicationInfo.vulkanVersion.majorVersion, mInitConfiguration.applicationInfo.vulkanVersion.minorVersion, mInitConfiguration.applicationInfo.vulkanVersion.patchVersion);
}

void Atlas::VulkanInstanceWrapper::enableValidationLayers(vkb::InstanceBuilder& instanceBuilder)
{
	const std::vector<VkLayerProperties> cAvailableLayers = GetAvailableLayers();

	std::ranges::for_each(mInitConfiguration.requestedValidationLayers, [&](const char* layerName) {
		InfoLog(std::format("Attempting to enable the requested validation layer: {}", layerName));

		// Ensure that the requested validation layer is available on the system
		if (!IsLayerSupported(layerName, cAvailableLayers)) {
			throw AException(std::format("Failed to enable the requested validation layer: {}. Are you sure it is installed on your system?", layerName));
		}

		instanceBuilder.enable_layer(layerName);
	});
}

void Atlas::VulkanInstanceWrapper::setupDebugMessenger(vkb::InstanceBuilder& instanceBuilder)
{
	if (mInitConfiguration.optionalDebugMessengerCallback.has_value()) {
		instanceBuilder.set_debug_callback(mInitConfiguration.optionalDebugMessengerCallback.value());
	}
	else {
#ifdef ATLAS_USE_VULKAN_DEFAULT_DEBUG_MESSENGER
		instanceBuilder.use_default_debug_messenger();
#else
		instanceBuilder.set_debug_callback(DefaultVulkanDebugCallback);
#endif
	}
}

Atlas::VulkanInstanceWrapper::VulkanInstanceWrapper(InitConfiguration initConfiguration)
	: mInitConfiguration(initConfiguration)
{
}

Atlas::VulkanInstanceWrapper::~VulkanInstanceWrapper()
{
	this->shutdown();
}

inline uint32_t Atlas::VulkanInstanceWrapper::GetAvailableLayerCount()
{
	// Get the layer count
	uint32_t layerCount = 0;
	
	if (const VkResult cEnumerationResult = vkEnumerateInstanceLayerProperties(&layerCount, nullptr); !IsSuccessfulEnumerationResult(cEnumerationResult)) {
		/*
		* TODO: Implement more precise error handling as there are multiple possible error codes that can occur from vkEnumerateInstanceLayerProperties. According to:
		* https://docs.vulkan.org/refpages/latest/refpages/source/vkEnumerateInstanceLayerProperties.html, below are the possible error codes that can occur:
		* 
		* 1. VK_ERROR_OUT_OF_DEVICE_MEMORY
		* 2. VK_ERROR_OUT_OF_HOST_MEMORY
		* 3. VK_ERROR_UNKNOWN
		* 4. VK_ERROR_VALIDATION_FAILED
		*/
		throw AException("Failed to get the number of available layers.");
	}

	return layerCount;
}

inline std::vector<VkLayerProperties> Atlas::VulkanInstanceWrapper::GetAvailableLayers()
{
	uint32_t layerCount = GetAvailableLayerCount();
	
	// Now, fill in a std::vector<VkLayerProperties> structure with the layer properties
	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	return layers;
}

bool Atlas::VulkanInstanceWrapper::IsLayerSupported(const char* layerName, std::vector<VkLayerProperties> const& availableLayers)
{
	auto it = std::find_if(availableLayers.begin(), availableLayers.end(),
		[layerName](const VkLayerProperties& layer) { return strcmp(layer.layerName, layerName) == 0; });

	return it != availableLayers.end();
}

void Atlas::VulkanInstanceWrapper::init() {
	auto instanceReturn = createVulkanInstance();

	// This can probably be an exception
	ATLAS_ASSERT(instanceReturn.has_value(), "Failed to create Vulkan instance.");

	mVulkanBootstrapInstance = instanceReturn.value();
	mVulkanInstance = instanceReturn.value().instance;
	mDebugMessenger = instanceReturn.value().debug_messenger;

	mbIsInitialized = true;
}

void Atlas::VulkanInstanceWrapper::shutdown()
{
	if (!mbIsInitialized) {
		WarnLog("Attempted to shut down a Vulkan instance that was not initialized or already shut down.");
		return;
	}

	if (mVulkanInstance == VK_NULL_HANDLE)
	{
		WarnLog("Attempted to shut down a Vulkan instance whose instance handle is null.");
		return;
	}

	if (mDebugMessenger != VK_NULL_HANDLE) {
		vkb::destroy_debug_utils_messenger(mVulkanInstance, mDebugMessenger);
	}

	vkDestroyInstance(mVulkanInstance, nullptr);

	mbIsInitialized = false;
}

VkInstance& Atlas::VulkanInstanceWrapper::getInstance() {
	return mVulkanInstance;
}

VkDebugUtilsMessengerEXT& Atlas::VulkanInstanceWrapper::getDebugMessenger() {
	return mDebugMessenger;
}

vkb::Instance& Atlas::VulkanInstanceWrapper::getVulkanBootstrapInstance() {
	return mVulkanBootstrapInstance;
}

Atlas::VulkanInstanceWrapper::operator const VkInstance& () const {
	return mVulkanInstance; 
}

Atlas::ELogLevel Atlas::TranslateVulkanLogLevel(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
{
	ELogLevel logLevel;

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		// Use ELogLevel::trace here because the trace log level is meant for verbose logging (usually this is logged in a log file anyways).
		logLevel = ELogLevel::trace;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		logLevel = ELogLevel::info;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		logLevel = ELogLevel::warn;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		logLevel = ELogLevel::error;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
		logLevel = ELogLevel::critical;
		break;
	default:
		logLevel = ELogLevel::info;
		break;
	}

	return logLevel;
}

VkBool32 Atlas::DefaultVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	spdlog::log(static_cast<spdlog::level::level_enum>(TranslateVulkanLogLevel(messageSeverity)), pCallbackData->pMessage);
	return VK_FALSE;
}
