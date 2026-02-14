/**************************************************************************************************
 * @file Instance.cpp
 *
 * @brief .
 *
 * @date September 2025
 *
 * @since v
 ***************************************************************************************************/
#include <optional>
#include <cstdint>
#include <memory>
#include <string>

#include "VulkanInstance.h"
#include "../../core/Core.h"
#include "../../core/Version.h"
#include "../../debugging/Logging.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

Atlas::VulkanInstanceWrapper::VulkanInstanceWrapper(InitConfiguration initConfiguration)
	: mInitConfiguration(initConfiguration)
{

}

Atlas::VulkanInstanceWrapper::~VulkanInstanceWrapper()
{
	this->shutdown();
}

uint16_t Atlas::VulkanInstanceWrapper::init() {
	uint16_t initResult = 0;

	vkb::InstanceBuilder instanceBuilder;

	instanceBuilder.set_app_name(mInitConfiguration.applicationInfo.name.c_str());
	instanceBuilder.set_engine_name(mInitConfiguration.engineName.c_str());

	instanceBuilder.set_engine_version(mInitConfiguration.atlasVersion.major, mInitConfiguration.atlasVersion.minor, mInitConfiguration.atlasVersion.patch);
	instanceBuilder.require_api_version(mInitConfiguration.applicationInfo.vulkanVersion.major, mInitConfiguration.applicationInfo.vulkanVersion.minor, mInitConfiguration.applicationInfo.vulkanVersion.patch);

	instanceBuilder.request_validation_layers(mInitConfiguration.bEnableValidationLayers);

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

	auto instanceReturn = instanceBuilder.build();

	ATLAS_ASSERT(instanceReturn.has_value(), "Failed to create Vulkan instance.");

	mVulkanBootstrapInstance = instanceReturn.value();
	mVulkanInstance = instanceReturn.value().instance;
	mDebugMessenger = instanceReturn.value().debug_messenger;

	return initResult;
}

void Atlas::VulkanInstanceWrapper::shutdown()
{
	vkb::destroy_debug_utils_messenger(mVulkanInstance, mDebugMessenger);
	vkDestroyInstance(mVulkanInstance, nullptr);
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
