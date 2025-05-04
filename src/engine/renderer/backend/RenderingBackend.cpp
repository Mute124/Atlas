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
#endif


#ifdef ATLAS_USE_VULKAN





void Atlas::VulkanRenderingBackend::init()
{


	if (vkCreateInstance(&this->mCreateInfo, nullptr, &this->mVulkanInstance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
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

