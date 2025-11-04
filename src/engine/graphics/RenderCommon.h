#pragma once

#include "../core/Core.h"
#include "../core/Common.h"
#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif



namespace Atlas {
#ifdef ATLAS_USE_VULKAN
	namespace Literals {

	}

	template<typename T_HANDLE>
	concept VulkanHandle = requires(T_HANDLE handle) {
		handle = VK_NULL_HANDLE;
	};

	/**
	 * @brief Checks if a provided Vulkan handle is valid.
	 * 
	 * @param handle The handle to check.
	 * 
	 * @tparam T_HANDLE The type of the handle to check.
	 * 
	 * @return @c true if the handle is @b not equal to @c VK_NULL_HANDLE, @c false otherwise.
	 * 
	 * @since v0.0.1
	 */
	template<VulkanHandle T_HANDLE>
	bool IsValidVulkanHandle(T_HANDLE handle) {
		return handle != VK_NULL_HANDLE;
	}

	/**
	 * @brief Checks if a provided Vulkan handle is invalid.
	 * 
	 * @param handle The handle to check.
	 * 
	 * @tparam T_HANDLE The type of the handle to check.
	 * 
	 * @return The inverted result of @c IsValidVulkanHandle(). In other words, @c true if the handle @b is equal to @c VK_NULL_HANDLE, @c false otherwise.
	 * 
	 * @since v0.0.1
	 */
	template<VulkanHandle T_HANDLE>
	bool IsInvalidVulkanHandle(T_HANDLE handle) {
		return !IsValidVulkanHandle<T_HANDLE>(handle);
	}

	template<VulkanHandle T_WRAPS>
	class AVulkanHandleWrapper : public Validatable {
	private:
		T_WRAPS mHandle;

	protected:

		virtual bool hasValidHandle() const { return IsValidVulkanHandle<T_WRAPS>(mHandle); }

		void setHandle(T_WRAPS handle) { mHandle = handle; }

		T_WRAPS& getHandleRef() { return mHandle; }

	public:

		explicit(false) AVulkanHandleWrapper(T_WRAPS handle) : mHandle(handle) {}

		explicit(false) AVulkanHandleWrapper(T_WRAPS& handle) : mHandle(handle) {}

		AVulkanHandleWrapper() = default;
		
		virtual ~AVulkanHandleWrapper() override = default;

		virtual bool isValid() const override {
			return Validatable::isValid() && hasValidHandle();
		}

		T_WRAPS getHandle() const { return mHandle; }

		explicit(false) virtual operator const T_WRAPS& () const {
			return mHandle;
		}
	};

#endif
}
