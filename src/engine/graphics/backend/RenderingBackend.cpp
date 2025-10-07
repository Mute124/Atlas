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

void Atlas::RenderingBackend::setAPIVersion(uint32_t major, uint32_t minor, uint32_t patch) {
	this->mAPIVersion.major = major;
	this->mAPIVersion.minor = minor;
	this->mAPIVersion.patch = patch; 
}

void Atlas::RenderingBackend::setAPIVersion(Version version) {
	this->setAPIVersion(version.major, version.minor, version.patch);
}

Atlas::Version Atlas::RenderingBackend::getAPIVersion() const {
	return this->mAPIVersion;
}

bool Atlas::RenderingBackend::areDebuggingToolsEnabled() const { 
	return this->mbEnableErrorChecking; 
}

bool Atlas::RenderingBackend::isErrorCheckingEnabled() const
{
	return this->mbEnableErrorChecking;
}

