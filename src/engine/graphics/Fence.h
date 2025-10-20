/**************************************************************************************************
 * @file Fence.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <vulkan/vulkan.h>

namespace Atlas {
	class Fence {
	private:


		VkFence mFence = VK_NULL_HANDLE;
		uint64_t mTimeout = UINT64_MAX;

	public:

		Fence() = default;

		VkResult reset();
		VkResult wait();

		VkFence getHandle() const { 
			return mFence;
		}
	};
}
