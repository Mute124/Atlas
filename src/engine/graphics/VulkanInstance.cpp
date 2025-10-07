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

#include "VulkanInstance.h"
#include "../core/Core.h"
#include "../core/Version.h"

#include <vulkan/vulkan_core.h>

Atlas::VulkanInstance::VulkanInstance()
{
#ifdef ATLAS_DEBUG
	mbEnableValidationLayers = true;
#endif

}

Atlas::VulkanInstance::~VulkanInstance()
{
	this->shutdown();
}

void Atlas::VulkanInstance::setVersion(const Version& cVulkanVersionRef) { 
	mVulkanVersion = cVulkanVersionRef; 
}

void Atlas::VulkanInstance::setApplicationName(std::string_view appName)
{
	mApplicationName = appName;
}

void Atlas::VulkanInstance::setEnableValidationLayers(const bool cbEnableValidationLayers) {
	mbEnableValidationLayers = cbEnableValidationLayers; 
}

void Atlas::VulkanInstance::setDebugCallback(VulkanDebugCallback debugMessengerCallback)
{
	mDebugMessengerCallback = std::make_optional(debugMessengerCallback);
}

uint16_t Atlas::VulkanInstance::init() {
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
		instanceBuilder.use_default_debug_messenger();
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

void Atlas::VulkanInstance::shutdown()
{
	vkb::destroy_debug_utils_messenger(mVulkanInstance, mDebugMessenger);
	vkDestroyInstance(mVulkanInstance, nullptr);
}

VkInstance& Atlas::VulkanInstance::getInstance() {
	return mVulkanInstance;
}

VkDebugUtilsMessengerEXT& Atlas::VulkanInstance::getDebugMessenger() {
	return mDebugMessenger;
}

vkb::Instance& Atlas::VulkanInstance::getVulkanBootstrapInstance() {
	return *mVulkanBootstrapInstance;
}

Atlas::VulkanInstance::operator const VkInstance& () const {
	return mVulkanInstance; 
}
