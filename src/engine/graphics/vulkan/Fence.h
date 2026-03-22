/**************************************************************************************************
 * @file Fence.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once
#include <cstdint>
#include <optional>
#include <format>
#include <chrono>

#include <vulkan/vulkan_core.h>

#include "PhysicalDevice.h"

#include "../../debugging/AException.h"
#include "../../debugging/Logging.h"

namespace Atlas {
	enum class EFenceStatus : uint8_t {
		Unknown = -1,
		DeviceLost,
		Ready,
		NotReady
	};

	class Fence final {
	private:
		VkDevice mOwner{ VK_NULL_HANDLE };
		VkFence mFence{ VK_NULL_HANDLE };
		uint64_t mTimeoutInNS{ UINT64_MAX };

		const VkAllocationCallbacks* cmAllocator{ nullptr };

		static inline void ParseAndThrowCreationError(VkResult error);

		static inline void ParseAndThrowWaitError(VkResult error);

		static inline void ParseAndThrowResetError(VkResult error);

	public:
		
		Fence(Device const& ownerDevice, const VkFenceCreateInfo* cCreateInfo, uint64_t timeoutInNS, const VkAllocationCallbacks* cAllocator = nullptr);

		Fence() = default;

		void destroy(VkDevice ownerDevice);

		void destroy();

		void wait(VkDevice ownerDevice, bool bWaitAll = true);

		void wait();

		void reset(VkDevice ownerDevice);

		void reset();

		void setTimeout(const uint64_t cNewTimeoutInNS) noexcept;

		EFenceStatus getStatus(VkDevice ownerDevice) const;

		bool hasDeviceBeenLost(VkDevice ownerDevice) const;

		uint64_t getTimeout() const noexcept;

		VkFence getFence() const noexcept;
	};
}
