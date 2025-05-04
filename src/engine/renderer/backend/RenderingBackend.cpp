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

Atlas::VulkanRenderingBackend::VulkanRenderingBackend(VkApplicationInfo const& appInfo, VkInstanceCreateInfo const& createInfo)
	: mAppInfo(appInfo), mCreateInfo(createInfo), IRenderingBackend(appInfo.pApplicationName)
{

}


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

}

#endif

bool Atlas::VulkanRenderingBackend::operator==(const VulkanRenderingBackend& other) const
{
	return mVulkanInstance == other.mVulkanInstance && mAppInfo == other.mAppInfo && mCreateInfo == other.mCreateInfo;
}
