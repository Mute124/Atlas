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
#include "../core/Core.h"
#include "../core/Version.h"
#include "../debugging/Logging.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan_core.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>


Atlas::VulkanInstanceWrapper::VulkanInstanceWrapper()
{
#ifdef ATLAS_DEBUG
	mbEnableValidationLayers = true;
#endif

}

Atlas::VulkanInstanceWrapper::~VulkanInstanceWrapper()
{
	this->shutdown();
}

void Atlas::VulkanInstanceWrapper::setVersion(const Version& cVulkanVersionRef) { 
	mVulkanVersion = cVulkanVersionRef; 
}

void Atlas::VulkanInstanceWrapper::setApplicationName(std::string_view appName)
{
	mApplicationName = appName;
}

void Atlas::VulkanInstanceWrapper::setEnableValidationLayers(const bool cbEnableValidationLayers) {
	mbEnableValidationLayers = cbEnableValidationLayers; 
}

void Atlas::VulkanInstanceWrapper::setDebugCallback(VulkanDebugCallback debugMessengerCallback)
{
	mDebugMessengerCallback = std::make_optional(debugMessengerCallback);
}

uint16_t Atlas::VulkanInstanceWrapper::init() {
	uint16_t initResult = 0;

	vkb::InstanceBuilder instanceBuilder;
	
	instanceBuilder.set_app_name(mApplicationName.c_str());

	// This function requires a const char*, but scEngineName is a std::string_view, therefore scEngineName.data() is used to convert to a const char*.
	instanceBuilder.set_engine_name(scEngineName.data()); 
	
	instanceBuilder.set_engine_version(ATLAS_VERSION_MAJOR, ATLAS_VERSION_MINOR, ATLAS_VERSION_PATCH);
	instanceBuilder.request_validation_layers(mbEnableValidationLayers);
	
	if (mDebugMessengerCallback.has_value()) {
		instanceBuilder.set_debug_callback(mDebugMessengerCallback.value());
	}
	else {


#ifdef ATLAS_USE_VULKAN_DEFAULT_DEBUG_MESSENGER
		instanceBuilder.use_default_debug_messenger();
#else
		instanceBuilder.set_debug_callback(DefaultVulkanDebugCallback);
#endif
	}

	instanceBuilder.require_api_version(mVulkanVersion.major, mVulkanVersion.minor, mVulkanVersion.patch);

	auto instanceReturn = instanceBuilder.build();
	
	//if (!instanceReturn.has_value()) {
	//	const std::error_code instanceBuildErrorCode = instanceReturn.error();
	//	
	//	initResult = instanceBuildErrorCode.value();

	//	std::cout << "Failed to build vulkan instance: " << instanceBuildErrorCode.message() << std::endl;

	//	return initResult;
	//}

	//vkb::Instance vulkanBootstrapInstance = instanceReturn.value();
	mVulkanBootstrapInstance = std::make_unique<vkb::Instance>(instanceReturn.value());

	vkb::Instance& vkBootstrapInstanceRef = getVulkanBootstrapInstance();

	mVulkanInstance = vkBootstrapInstanceRef.instance;
	mDebugMessenger = vkBootstrapInstanceRef.debug_messenger;

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
	return *mVulkanBootstrapInstance;
}

Atlas::VulkanInstanceWrapper::operator const VkInstance& () const {
	return mVulkanInstance; 
}

Atlas::LogLevel Atlas::TranslateVulkanLogLevel(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
{
	LogLevel logLevel;

	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		// Use LogLevel::trace here because the trace log level is meant for verbose logging (usually this is logged in a log file anyways).
		logLevel = LogLevel::trace;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		logLevel = LogLevel::info;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		logLevel = LogLevel::warn;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		logLevel = LogLevel::err;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
		logLevel = LogLevel::critical;
		break;
	default:
		logLevel = LogLevel::info;
		break;
	}

	return logLevel;
}

VkBool32 Atlas::DefaultVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	spdlog::log(TranslateVulkanLogLevel(messageSeverity), pCallbackData->pMessage);	
	return VK_FALSE;
}
