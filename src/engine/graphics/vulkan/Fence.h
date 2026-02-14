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
#include <cstdint>
#include <optional>

#include <vulkan/vulkan_core.h>

#include "PhysicalDevice.h"

namespace Atlas {
	class Fence final {
	private:
		VkFence mFence{ VK_NULL_HANDLE };
		uint64_t mTimeout{ UINT64_MAX };

	public:

		Fence(const VkFence& mFence, const uint64_t& mTimeout)
			: mFence(mFence), mTimeout(mTimeout)
		{
		}

		Fence() = default;

		~Fence() {
			destroy();
		}

		void reset(Device const& device) {
			vkResetFences(device.getHandle(), 1, &mFence);
		}

		void reset() {
		}

		void wait(Device const& device) {
			vkWaitForFences(device.getHandle(), 1, &mFence, VK_TRUE, mTimeout);
		}

		void wait()
		{

		}

		void destroy(Device const& device) {
			vkDestroyFence(device.getHandle(), mFence, nullptr);
		}

		void destroy() {
			destroy(*Device::GetMainHandle().get());
		}

		VkFence getHandle() const { 
			return mFence;
		}
	};
}
