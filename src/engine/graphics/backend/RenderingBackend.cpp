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

void Atlas::ARenderingBackend::setAPIVersion(Version version) {
	this->mAPIVersion = version;
	//this->setAPIVersion(version.major, version.minor, version.patch);
}

Atlas::Version Atlas::ARenderingBackend::getAPIVersion() const {
	return this->mAPIVersion;
}

bool Atlas::ARenderingBackend::areDebuggingToolsEnabled() const { 
	return this->mbEnableErrorChecking; 
}

bool Atlas::ARenderingBackend::isErrorCheckingEnabled() const
{
	return this->mbEnableErrorChecking;
}

