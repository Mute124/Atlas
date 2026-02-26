/**************************************************************************************************
 * @file Viewport.h
 * 
 * @brief Provides code to interact with a vulkan viewport.
 * 
 * @date February 2026
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include "../../core/Core.h"
#include <vulkan/vulkan_core.h>


namespace Atlas {

	/**
	 * @brief Wraps a VkViewport instance, providing functions to interact with it, along with some safety checks. 
	 * 
	 * @todo Flesh this class out.
	 * 
	 * @since v0.0.1
	 */
	class Viewport {
	private:
		/**
		 * @brief The viewport instance.
		 * 
		 * @since v0.0.1
		 */
		VkViewport mViewport{};

		/**
		 * @brief Implementation of dynamically setting the viewport. This is a wrapper for vkCmdSetViewport.
		 * 
		 * @note This function does not have any checks in it, only the public function does.
		 * 
		 * @param commandBuffer The command buffer to set the viewport on. This must be in recording mode.
		 * @param viewport The viewport to tell Vulkan to use.
		 * 
		 * @since v0.0.1
		 */
		static inline void SetCommandBufferViewport(VkCommandBuffer commandBuffer, VkViewport const* viewport);

	public:
		
		/**
		 * @brief Constructs a new Viewport.
		 * 
		 * @param mViewport A fully filled VkViewport struct instance.
		 * 
		 * @since v0.0.1
		 */
		ATLAS_EXPLICIT Viewport(const VkViewport& mViewport);

		Viewport() = default;

		/**
		 * @brief Sets the Vulkan viewport for the given command buffer.
		 * 
		 * @pre The command buffer must be in recording mode prior to this function being called.
		 * 
		 * @todo Add more checks to ensure that proper usage is done.
		 * 
		 * @param commandBuffer The Vulkan command buffer to set the viewport on. This must not be `VK_NULL_HANDLE`.
		 * 
		 * @since v0.0.1
		 */
		void set(VkCommandBuffer commandBuffer);
		
		/**
		 * @brief Get the Vulkan viewport.
		 *
		 * @return `VkViewport` The Vulkan viewport.
		 *
		 * @since v0.0.1
		 */
		VkViewport get() const;
	};
}
