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
		T_WRAPS mHandle{ VK_NULL_HANDLE };

	protected:

		virtual bool hasValidHandle() const { return IsValidVulkanHandle<T_WRAPS>(mHandle); }

		void setHandle(T_WRAPS handle) { mHandle = handle; }

		T_WRAPS& getHandleRef() { return mHandle; }

	public:

		explicit(false) AVulkanHandleWrapper(T_WRAPS handle) : mHandle(handle) {}

		//explicit(false) AVulkanHandleWrapper(T_WRAPS& handle) : mHandle(handle) {}

		AVulkanHandleWrapper() = default;
		
		virtual ~AVulkanHandleWrapper() override = default;

		virtual bool isValid() const override {
			return Validatable::isValid() && hasValidHandle();
		}

		T_WRAPS getHandle() const { return mHandle; }

		explicit(false) operator const T_WRAPS& () const {
			return mHandle;
		}

		explicit(false) operator T_WRAPS& () {
			return mHandle;
		}

		explicit(false) operator bool() const { return isValid(); }
	};

	template<VulkanHandle T_WRAPS, typename T_VKB_EQUIVALENT, typename T_VKB_EQUIVALENT_BUILDER>
	class AVulkanCompositeHandleWrapper : public AVulkanHandleWrapper<T_WRAPS> {
	private:
		T_VKB_EQUIVALENT mVkbHandle;



	protected:
		//static inline T_VKB_EQUIVALENT& GenerateVkbEquivalent(T_VKB_EQUIVALENT_BUILDER builder) {
		//	return builder.build().value();
		//}

		void setVkbHandle(T_VKB_EQUIVALENT vkbHandle) { mVkbHandle = vkbHandle; }

		bool hasValidVkbHandle() const {
			// VKB classes have an implicit operator to T_WRAPS 
			// (assuming T_WRAPS is the same type as T_VKB_EQUIVALENT)
			return IsValidVulkanHandle<T_WRAPS>(mVkbHandle);
		}
	public:

		AVulkanCompositeHandleWrapper(T_WRAPS handle, T_VKB_EQUIVALENT vkbHandle) 
			: AVulkanHandleWrapper<T_WRAPS>(handle), mVkbHandle(vkbHandle) {}
		
		//AVulkanCompositeHandleWrapper(T_WRAPS& handle, T_VKB_EQUIVALENT& vkbHandle) 
		//	: AVulkanHandleWrapper<T_WRAPS>(handle), mVkbHandle(vkbHandle) {}

		explicit AVulkanCompositeHandleWrapper(T_VKB_EQUIVALENT_BUILDER builder) 
			: AVulkanHandleWrapper<T_WRAPS>(), mVkbHandle(builder.build().value()) {}

		AVulkanCompositeHandleWrapper() = default;
		
		virtual ~AVulkanCompositeHandleWrapper() override = default;

		virtual bool isValid() const override {
			return AVulkanHandleWrapper<T_WRAPS>::isValid() && hasValidVkbHandle();
		}

		T_VKB_EQUIVALENT& getVkbHandle() { 
			return mVkbHandle;
		}

		T_VKB_EQUIVALENT* getVkbHandlePtr() {
			return &mVkbHandle;
		}

		T_VKB_EQUIVALENT* safeGetVkbHandlePtr() {
			T_VKB_EQUIVALENT* ptr = getVkbHandlePtr();
			
			ATLAS_ASSERT(ptr != nullptr, "Vulkan handle is not valid!");

			return ptr;
		}

		explicit(false) operator const T_VKB_EQUIVALENT& () const { return getVkbHandle(); }

		 T_VKB_EQUIVALENT* operator->() { 
			 return safeGetVkbHandlePtr();
		 }
	};

#endif
}
