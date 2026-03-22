/**************************************************************************************************
 * @file Fence.h
 * 
 * @brief Provides declarations for wrapping a Vulkan fence.
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
	/**
	 * @brief The status of a fence. 
	 * 
	 * @remarks Usually, it is used to determine whether a fence is ready or not, but may be used for other things as well.
	 * 
	 * @since v0.0.1
	 */
	enum class EFenceStatus : uint8_t {
		/**
		 * @brief The status of the fence is unknown. If you encounter this during development, please file a bug report as
		 * this is most likely a programming error. However, please ensure your code is not causing this before doing so.
		 * 
		 * @since v0.0.1
		 */
		Unknown = 0,

		/**
		 * @brief The result of the `vkGetFenceStatus` call was `VK_ERROR_DEVICE_LOST`. This value is something you do not want to willingly
		 * encounter as this will open up a whole can of worms and a lot of trouble. This usually means that the GPU or driver the application
		 * and engine were running on has crashed. This should be treated as a fatal error.
		 * 
		 * @since v0.0.1
		 */
		DeviceLost,

		/**
		 * @brief The fence is ready to use, in the `VK_SUCCESS` state, and there is @b no GPU work pending.
		 * 
		 * @since v0.0.1
		 */
		Ready,

		/**
		 * @brief The fence is not ready to use, in the `VK_NOT_READY` state, and there is GPU work pending.
		 * 
		 * @since v0.0.1
		 */
		NotReady
	};

	/**
	 * @brief Wraps a Vulkan fence, providing useful functions while ensuring safe usage of the fence during its lifetime. 
	 * 
	 * @todo Finish documentation, see if more comments are needed, and make the use of exceptions better.
	 * 
	 * @since v0.0.1
	 */
	class Fence final {
	private:
		
		/**
		 * @brief The device that owns this fence. This must be a valid device.
		 * 
		 * @since v0.0.1
		 */
		VkDevice mOwner{ VK_NULL_HANDLE };

		/**
		 * @brief The fence itself. This is created in the constructor.
		 * 
		 * @since v0.0.1
		 */
		VkFence mFence{ VK_NULL_HANDLE };

		/**
		 * @brief How long, <b>in nanoseconds</b>, to wait for the fence before timing out. Set to `UINT64_MAX` to wait indefinitely (not really,
		 * but eh, 584.94241735507205249 years is close enough).
		 * 
		 * @note Even though this can be as long as you want, and is set to `UINT64_MAX` by default, it is recommended that you set your own timeout. 
		 * You may want to use a timeout of at least a few seconds or so, depending on your application. The file's documentation has more details.
		 * about fence timeouts.
		 * 
		 * @since v0.0.1
		 */
		uint64_t mTimeoutInNS{ UINT64_MAX };

		/**
		 * @brief The allocation callbacks to use when creating and destroying the fence. This is optional, and can be left `nullptr` if you don't
		 * have anything special to pass in.
		 * 
		 * @since v0.0.1
		 */
		const VkAllocationCallbacks* cmAllocationCallbacks{ nullptr };

		/**
		 * @brief Throws an exception based on the error that vkCreateFence() returned. This function is really only used in the constructor,
		 * and was created to make the code more readable. Don't ask me why, Khronos does what Khronos does.
		 * 
		 * @note The `vkCreateFence()` documentation's list of possible error codes states `VK_ERROR_VALIDATION_FAILED` as a possible error code,
		 * and it is a valid code, but on some situations it is not, therefore, `VK_ERROR_VALIDATION_FAILED_EXT` is being used instead of 
		 * `VK_ERROR_VALIDATION_FAILED`. I do not know if this will cause issues down the line, and is worth looking into.
		 * 
		 * @param error The error code returned by `vkCreateFence()`.
		 * 
		 * @since v0.0.1
		 */
		static inline void ParseAndThrowCreationError(VkResult error);

		/**
		 * @brief Throws an exception based on the error that `vkWaitForFences()` returned. This function is really only used in the `wait()`
		 * function, and was created to make the code more readable.
		 * 
		 * @note Please see the first note on `ParseAndThrowCreationError()` for the reason why `VK_ERROR_VALIDATION_FAILED_EXT` is being used
		 * instead of `VK_ERROR_VALIDATION_FAILED`, despite Khronos' own documentation using the latter. Don't ask me why, Khronos does what
		 * Khronos does.
		 * 
		 * @param error The error code returned by `vkWaitForFences()`.
		 * 
		 * @since v0.0.1
		 */
		static inline void ParseAndThrowWaitError(VkResult error);

		/**
		 * @brief Throws an exception based on the error that `vkResetFences()` returned in the `reset()` function.
		 * 
		 * @note Please see the first note on `ParseAndThrowCreationError()` for the reason why `VK_ERROR_VALIDATION_FAILED_EXT` is being used
		 * instead of `VK_ERROR_VALIDATION_FAILED`, despite Khronos' own documentation using the latter. Don't ask me why, Khronos does what
		 * Khronos does.
		 * 
		 * @param error The error code returned by `vkResetFences()`.
		 * 
		 * @since v0.0.1
		 */
		static inline void ParseAndThrowResetError(VkResult error);

	public:
		/**
		 * @brief Constructs a fence, creating it on the specified device. 
		 * 
		 * @param ownerDevice The device that will own the fence. <b>This must be a valid device for the entire lifetime of the fence!</b>
		 * @param cCreateInfo The fence create info. This cannot be `nullptr`.
		 * @param timeoutInNS How long, in nanoseconds, to wait for the fence before timing out.
		 * @param cAllocator The allocation callbacks to use when creating and destroying the fence. This is optional, and can safely be left
		 * `nullptr` if you don't have anything special to pass in.
		 * 
		 * @since v0.0.1
		 */
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
