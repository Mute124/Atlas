/**************************************************************************************************
 * @file VulkanRenderingBackend.cpp
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#include <stdexcept>

#include "../../core/Core.h"

#include "RenderingBackend.h"

#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan_core.h>
	#include <VkBootstrap.h>
	
	#ifdef ATLAS_USE_SDL2
		#include <SDL_vulkan.h>
	#endif

#endif

void Atlas::RenderingBackend::setAPIVersion(APIVersion version) {
	this->setAPIVersion(version.major, version.minor, version.patch);
}

Atlas::RenderingBackend::APIVersion Atlas::RenderingBackend::getAPIVersion() const { 
	return this->mAPIVersion;
}

bool Atlas::RenderingBackend::areDebuggingToolsEnabled() const { 
	return this->mbEnableErrorChecking; 
}

bool Atlas::RenderingBackend::isErrorCheckingEnabled() const
{
	return this->mbEnableErrorChecking;
}

#ifdef ATLAS_USE_VULKAN

uint16_t Atlas::VulkanRenderingBackend::initInstance(const APIVersion& cAPIVersionRef, bool cbEnableValidationLayers, std::string appName)
{
	vkb::InstanceBuilder instanceBuilder;

	auto instanceReturn = instanceBuilder.set_app_name("Example Vulkan Application")
		.request_validation_layers(cbEnableValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(cAPIVersionRef.major, cAPIVersionRef.minor, cAPIVersionRef.patch)
		.build();

	vkb::Instance vulkanInstance = instanceReturn.value();

	this->mVulkanInstance = vulkanInstance.instance;
	this->mVulkanDebugMessenger = vulkanInstance.debug_messenger;
	return 0;
}

void Atlas::VulkanRenderingBackend::init()
{
	const bool cbEnableValidationLayers = isErrorCheckingEnabled();
	const APIVersion cApiVersion = getAPIVersion();

	vkb::InstanceBuilder instanceBuilder;

	auto instanceReturn = instanceBuilder.set_app_name("Example Vulkan Application")
		.request_validation_layers(cbEnableValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(cApiVersion.major, cApiVersion.minor, cApiVersion.patch)
		.build();

	vkb::Instance vulkanInstance = instanceReturn.value();

	this->mVulkanInstance = vulkanInstance.instance;
	this->mVulkanDebugMessenger = vulkanInstance.debug_messenger;

	



	//if (vkCreateInstance(&this->mCreateInfo, nullptr, &this->mVulkanInstance) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to create instance!");
	//}
}

void Atlas::VulkanRenderingBackend::update()
{

}

void Atlas::VulkanRenderingBackend::shutdown()
{
	vkDestroyInstance(this->mVulkanInstance, nullptr);
}

bool Atlas::VulkanRenderingBackend::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());


	for (const char* layerName : this->mValidationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		
		if (!layerFound) {
			return false;
		}
	}

	return true;
}

#endif

