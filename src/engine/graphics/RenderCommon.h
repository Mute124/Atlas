#pragma once

#include "../core/Core.h"
#include "../core/Common.h"
#ifdef ATLAS_USE_VULKAN
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
#endif



namespace Atlas {
	namespace Literals {
		// Empty for now

		constexpr double DEFAULT_NEAR_CULL_DISTANCE = 0.1;
		constexpr double DEFAULT_FAR_CULL_DISTANCE = 1000.0;
	}

	enum class EDrawingMode {
		Deferred,
		Forward
	};

	enum class ECameraProjection {
		Orthographic,
		Perspective
	};

	enum class ECameraPerspectiveMode {
		FirstPerson,
		ThirdPerson
	};

	enum class EGeometryProjection {
		Third_Dimension,
		Second_Dimension
	};

	//enum class EDrawPerspective {
	//	LeftHanded,
	//	RightHanded
	//};

	enum class EDrawStage {
		PreDraw,
		Draw,
		PostDraw
	};
	
	struct CullRange {
		double near{ Literals::DEFAULT_NEAR_CULL_DISTANCE };
		double far{ Literals::DEFAULT_FAR_CULL_DISTANCE };
	};

	struct ObjectCullInfo {
		CullRange cullRange{};
		bool bAllowCulling{ true }; // this is to allow per-object culling

	};

	class Culler {
	public:


	private:
		CullRange mCullDistance{};

	public:
		
		explicit Culler(CullRange cullDistance) : mCullDistance(cullDistance) {}

		Culler() = default;

		bool shouldCull(double distance, ObjectCullInfo const& cullInfo) const {
			return distance < mCullDistance.near || distance > mCullDistance.far;
		}
	};

	class ICamera {
	public:
	};

	class ADrawingMode {
	private:
		EGeometryProjection mGeometryProjection = EGeometryProjection::Third_Dimension;

	public:
		
		ADrawingMode() = default;

		virtual void begin() = 0;

		virtual void end() = 0;
	};

	class Layer;

	class IRenderer : public Validatable {
	public:

		virtual void beginDrawing() = 0;

		virtual void endDrawing() = 0;
	};
	

#ifdef ATLAS_USE_VULKAN


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

	template<typename T_WRAPS>
	class AVulkanHandleWrapper : public InitializableAndValidatable /*: public HandleWrapperBase<T_WRAPS>*/ {
	private:
		T_WRAPS mHandle{ VK_NULL_HANDLE };

	protected:

		void setHandle(T_WRAPS handle) { mHandle = handle; }

		T_WRAPS& getHandleRef() { return mHandle; }

	public:

		//explicit(false) AVulkanHandleWrapper(T_WRAPS handle) : mHandle(handle) {}

		//explicit(false) AVulkanHandleWrapper(T_WRAPS& handle) : mHandle(handle) {}


		//AVulkanHandleWrapper(T_WRAPS handle, T_WRAPS invalidHandle) : HandleWrapperBase<T_WRAPS>(handle, invalidHandle) {}

		explicit(false) AVulkanHandleWrapper(T_WRAPS& handle) : InitializableAndValidatable(true, true), mHandle(handle) {}

		AVulkanHandleWrapper() = default;
		
		virtual ~AVulkanHandleWrapper() override = default;
		
		virtual bool hasValidHandle() const { return IsValidVulkanHandle<T_WRAPS>(mHandle); }

		virtual bool isValid() const override {

			return InitializableAndValidatable::isValid() && this->hasValidHandle();
		}

		T_WRAPS getHandle() const { return mHandle; }

		T_WRAPS* getHandlePtr() { return &mHandle; }

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
