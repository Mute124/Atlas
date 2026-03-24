/**************************************************************************************************
 * @file Camera.h
 * 
 * @brief .
 * 
 * @date March 2026
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <cstdint>



#include "../../core/Math.h"

namespace Atlas {
	enum class ECameraProjectionType : uint8_t {
		Orthographic,
		Perspective,
		Free
	};

	enum class ECameraType : uint8_t {
		FirstPerson,
		ThirdPerson
	};

	// This struct exists to group the near and far planes together and make sure they stay together
	struct ClippingInfo {
		float nearPlane{ 1.1f };
		float farPlane{ 10000.0f };

		ClippingInfo(float nearPlane, float farPlane)
			: nearPlane(nearPlane), farPlane(farPlane)
		{
		}

		ClippingInfo() = default;
	};



	class Camera {
	private:
		bool mbIsViewDirty{ true };
		bool mbIsProjectionDirty{ true };

		float mFOV;
		float mAspectRatio;
		ClippingInfo mClippingInfo{};

		ECameraProjectionType mProjectionType{ ECameraProjectionType::Perspective };
		ECameraType mCameraType{ ECameraType::FirstPerson };

		Vector3 mPosition{ 0.0f, 0.0f, 0.0f };

		Matrix4 mViewMatrix{ Matrix4(1.0f) };
		Matrix4 mProjectionMatrix{ Matrix4(1.0f) };
	public:

		const Matrix4& getViewMatrix() const { return mViewMatrix; }
		const Matrix4& getProjectionMatrix() const { return mProjectionMatrix; }
	};


}
