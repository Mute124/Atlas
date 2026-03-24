/**************************************************************************************************
 * @file Viewport.h
 * 
 * @brief Provides code to interact with a vulkan viewport.
 * 
 * @todo Review documentation of things in this file.
 * 
 * @date February 2026
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include "../../core/Core.h"

#include <vulkan/vulkan_core.h>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Atlas {


	/**
	 * @brief Wraps a `VkViewport` instance, providing functions to interact with it, along with some safety checks.
	 * Furthermore, it allows you to skip the boilerplate code for dynamically setting the viewport. 
	 * 
	 * @note Before setting the viewport, please note that The viewport's position should be considered the <b>top
	 * left corner</b> of the viewport. Additionally, the viewport's minimum and maximum depth @b can be set to
	 * anything you want since the @b minimum depth can be less than, equal to, and/or greater than the @b maximum
	 * depth. This was noted in the Vulkan specifications.
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
		 * @deprecated Use `applyViewportToCommandBuffer` instead. This was removed because it had no reason
		 * to exist.
		 * 
		 * @note This function does not have any checks in it, only the public function does.
		 * 
		 * @param commandBuffer The command buffer to set the viewport on. This must be in recording mode.
		 * @param viewport The viewport to tell Vulkan to use.
		 * 
		 * @since v0.0.1
		 */
		static void SetCommandBufferViewport(VkCommandBuffer commandBuffer, VkViewport const* viewport);
	public:
		
		/**
		 * @brief Constructs a new Viewport.
		 * 
		 * @param mViewport A fully filled VkViewport struct instance.
		 * 
		 * @since v0.0.1
		 */
		ATLAS_EXPLICIT Viewport(const VkViewport& mViewport);

		/**
		 * @brief Constructs a new Viewport with the given parameters.
		 * 
		 * @param position The top left corner of the viewport
		 * @param size The size of the viewport
		 * @param minDepth 
		 * @param maxDepth
		 * 
		 * 
		 * @since v0.0.1
		 */
		Viewport(glm::vec2 const& position, glm::vec2 const& size, float minDepth = 0.0f, float maxDepth = 1.0f);

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
		void applyViewportToCommandBuffer(VkCommandBuffer commandBuffer);

		/**
		 * @brief Sets the top left corner of the viewport using a 2D vector. This function makes a call to the
		 * overload `setPosition(float, float)`.
		 * 
		 * @param position A 2D vector representing the top left corner of the viewport
		 * 
		 * @since v0.0.1
		 */
		void setPosition(glm::vec2 const& position);

		/**
		 * @brief Sets the top left corner of the viewport using the x and y coordinates. This function is
		 * where the actual setting of the viewport's position happens.
		 * 
		 * @note This function is provided for convenience.
		 * 
		 * @param x The new `x` coordinate of the top left corner of the viewport. 
		 * @param y The new `y` coordinate of the top left corner of the viewport.
		 * 
		 * @since v0.0.1
		 */
		void setPosition(float x, float y) noexcept;

		/**
		 * @brief Sets the `width` and `height` values of `mViewport` using a 2D vector. This function makes a call to the overload
		 * `setSize(float, float)`.
		 * 
		 * @param size A 2D vector with the new width and height of the viewport.
		 * 
		 * @since v0.0.1
		 */
		void setSize(glm::vec2 const& size);

		/**
		 * @brief Sets `mViewport.width` and `mViewport.height` using the given width and height.
		 * 
		 * @note This function is provided for convenience.
		 * 
		 * @param width The new width of the viewport.
		 * @param height The new height of the viewport.
		 * 
		 * @since v0.0.1
		 */
		void setSize(float width, float height) noexcept;

		/**
		 * @brief Sets the depth range of the viewport within `mViewport`. Whether or not the depth range is valid, and which
		 * value is near or far, is up to the user's discretion and how the engine was set up.
		 * 
		 * @param minDepth The new minimum depth of the viewport.
		 * @param maxDepth The new maximum depth of the viewport.
		 * 
		 * @since v0.0.1
		 */
		void setDepthRange(float minDepth, float maxDepth) noexcept;
		
		/**
		 * @brief Gets the top left corner of the viewport stored within `mViewport`.
		 * 
		 * @return A 2D vector containing the current top left corner of the viewport
		 * 
		 * @since v0.0.1
		 */
		glm::vec2 getPosition() const;

		/**
		 * @brief Gets the size of the viewport stored within `mViewport`.
		 * 
		 * @return A 2D vector containing the current width and height of the viewport.
		 * 
		 * @since v0.0.1
		 */
		glm::vec2 getSize() const;

		/**
		 * @brief Gets the minimum depth value stored within `mViewport`.
		 * 
		 * @return The current minimum depth
		 * 
		 * @since v0.0.1
		 */
		float getMinDepth() const noexcept;

		/**
		 * @brief Gets the maximum depth value stored within `mViewport`.
		 * 
		 * @return The current maximum depth
		 * 
		 * @since v0.0.1
		 */
		float getMaxDepth() const noexcept;

		/**
		 * @brief Gets the currently stored Vulkan viewport.
		 *
		 * @return `VkViewport` The Vulkan viewport.
		 *
		 * @since v0.0.1
		 */
		VkViewport getViewport() const noexcept;
	};
}
