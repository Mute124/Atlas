/**************************************************************************************************
 * @file Camera.h
 * 
 * @brief .
 * 
 * @date March 2026
 * 
 * @since v
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ***************************************************************************************************/
#pragma once

#include <cstdint>

#include "../../core/Math.h"

namespace Atlas {
	/**
	 * @brief Describes the @b projection type of a @c Camera instance. 
	 * 
	 * @since v0.0.1
	 */
	enum class ECameraProjectionType : uint8_t {
		/**
		 * @brief Denotes an orthographic projection.
		 * 
		 * @since v0.0.1
		 */
		Orthographic,

		/**
		 * @brief Denotes a perspective projection. 
		 * 
		 * @since v0.0.1
		 */
		Perspective,

		/**
		 * @brief Denotes a free projection, with no constraints on it's behavior. This is also known as
		 * a @b Freecam.
		 * 
		 * @since v0.0.1
		 */
		Free
	};

	/**
	 * @brief Describes the @b point-of-view (POV for short) mode of a @c Camera instance. 
	 * 
	 * @remarks As of v0.0.1, only FirstPerson and ThirdPerson modes are supported, but the idea of
	 * supporting more is currently being considered, such as a top down view, over the shoulder, etc.
	 * However, this is not a priority right now, as first person and third person are the most common
	 * use cases for a camera in a game.
	 * 
	 * @since v0.0.1
	 */
	enum class ECameraPOV : uint8_t {
		/**
		 * @brief Denotes a camera in first person point-of-view. When this is used, the camera should
		 * be placed as if the camera is the player or character's eye.
		 * 
		 * @since v0.0.1
		 */
		FirstPerson,

		/**
		 * @brief Denotes a camera in third person point of view. When this is used, the camera should
		 * be placed as if the camera was looking at the player or character from some sort of distance.
		 * 
		 * @since v0.0.1
		 */
		ThirdPerson
	};

	/**
	 * @brief Groups the near and far planes together to make sure they stay together. It is important to note that it is recommended to have the far plane be less than the near plane
	 * to avoid any z-fighting issues. Anything that is not placed between the near and far planes will @b NOT be rendered.
	 * 
	 * @todo Consider doing the following: 
	 * - Adding a check within the constructor to make sure the far plane is less than the near plane, thereby avoiding z-fighting caused by developer error.
	 * 
	 * @since v0.0.1
	 */
	struct ViewFustrum {
		/**
		 * @brief The 'near' clipping plane. As mentioned in the class description, it is recommended to have the far plane be less than the near plane.
		 * 
		 * @since v0.0.1
		 */
		float nearPlane{ 10000.0f };

		/**
		 * @brief The 'far' clipping plane. As mentioned in the class description, it is recommended to have the far plane be less than the near plane.
		 * 
		 * @since v0.0.1
		 */
		float farPlane{ 10.1f };

		/**
		 * @brief Constructs a new @c ViewFustrum with the given near and far planes. This constructor also provides a utility boolean parameter to swap the near and far planes
		 * if the @b far plane is <b>less than or equal to</b> the @b near plane.
		 * 
		 * @param nearPlane The value to set the `nearPlane` variable to.
		 * @param farPlane The value to set the `farPlane` variable to.
		 * @param bEnforceInvertedPlanes Whether or not to swap the near and far planes if the far plane is less than or equal to the near plane. By default, this is set to @c true.
		 * 
		 * @since v0.0.1
		 */
		ViewFustrum(float nearPlane, float farPlane, bool bEnforceInvertedPlanes = true)
			: nearPlane(nearPlane), farPlane(farPlane)
		{
			if (bEnforceInvertedPlanes && farPlane >= nearPlane) {
				std::swap(nearPlane, farPlane);
			}
		}

		ViewFustrum() = default;
	};

	/**
	 * @brief Represents a @b dumb camera that does not have any actual rendering capabilities, nor
	 * does it react to any user input. For anything to change, the getter and setter functions must
	 * be used.
	 * 
	 * @since v0.0.1
	 */
	class Camera {
	private:

		/**
		 * @brief Whether or not the view matrix needs to be recalculated. If this is set to true, the @c CameraController
		 * (@c CameraController) will recalculate the view matrix when possible, setting this to false when done.
		 * 
		 * @since v0.0.1
		 */
		bool mbIsViewDirty{ true };

		/**
		 * @brief The field of view (FOV) <b>in radians</b>.
		 * 
		 * @note As an edge case, this can also be used as a zoom factor.
		 * 
		 * @warning This should not be confused with FOVY, which is the vertical field of view. 
		 * 
		 * @since v0.0.1
		 */
		float mFOV;

		/**
		 * @brief A @c float defining the proportional relationship between an image or screen's width and its height.
		 * 
		 * @note When calculating the aspect ratio, it is recommended to use the render area's width divided by it's
		 * height. 
		 * 
		 * @since v0.0.1
		 */
		float mAspectRatio;

		/**
		 * @brief The view fustrum of this camera. Anything not within the view fustrum will not be rendered.
		 * 
		 * @since v0.0.1
		 */
		ViewFustrum mViewFustrum{};

		/**
		 * @brief The type of projection to use. This is set to @c ECameraProjectionType::Perspective by default as that
		 * is the most common projection type. Please see @c ECameraProjectionType for more information on projections.
		 * 
		 * @important The value you set this to will greatly affect how the camera behaves. Please be aware of this.
		 * 
		 * @since v0.0.1
		 */
		ECameraProjectionType mProjectionType{ ECameraProjectionType::Perspective };

		/**
		 * @brief The point of view mode of the camera.
		 * 
		 * @since v0.0.1
		 */
		ECameraPOV mCameraPOV{ ECameraPOV::FirstPerson };

		/**
		 * @brief As the name suggests, this is the position of the camera in @b world-space. The Y-Axis should be the
		 * vertical (up or down) axis.
		 * 
		 * @since v0.0.1
		 */
		Vector3 mWorldPosition{ 0.0f, 0.0f, 0.0f };

		/**
		 * @brief The matrix that represents the camera's view. If this needs to be recalculated, use @c markViewDirty()
		 * to tell the camera's controller (@c CameraController) to recalculate it.
		 * 
		 * @since v0.0.1
		 */
		Matrix4 mViewMatrix{ Matrix4(1.0f) };

		/**
		 * @brief The matrix that represents the camera's projection.
		 * 
		 * @since v0.0.1
		 */
		Matrix4 mProjectionMatrix{ Matrix4(1.0f) };

	public:

		/**
		 * @brief Constructs a new @c Camera instance. 
		 * 
		 * @param FOV				The field of view (FOV) <b>in radians</b>.
		 * @param aspectRatio		The proportional relationship between an image or screen's width and its height.
		 * @param viewFustrum		The view fustrum of this camera.
		 * @param projectionType	The type of projection to use.
		 * @param cameraPOVMode		The point of view mode of the camera.
		 * 
		 * @since v0.0.1
		 */
		Camera(float FOV, float aspectRatio, ViewFustrum const& viewFustrum, ECameraProjectionType projectionType, ECameraPOV cameraPOVMode)
			: mFOV(FOV), mAspectRatio(aspectRatio), mViewFustrum(viewFustrum), mProjectionType(projectionType), mCameraPOV(cameraPOVMode) {}

		Camera() = default;

		/**
		 * @brief Sets the view matrix as dirty, telling the camera controller (@c CameraController) to recalculate it when
		 * possible. When the @c CameraController recalculates the view matrix, it will set this to false automatically.
		 *
		 * @since v0.0.1
		 */
		void markViewDirty() {
			mbIsViewDirty = true;
		}

		/**
		 * @brief Sets the field of view.
		 * 
		 * @param newFOV
		 * 
		 * @since v0.0.1
		 */
		void setFOV(float newFOV) {
			mFOV = newFOV;
		}

		/**
		 * @brief Sets the aspect ratio.
		 * 
		 * @param newAspectRatio The new aspect ratio
		 * 
		 * @since v0.0.1
		 */
		void setAspectRatio(float newAspectRatio) {
			mAspectRatio = newAspectRatio;
		}

		/**
		 * @brief Sets the view fustrum.
		 * 
		 * @param newViewFustrum The new view fustrum
		 * 
		 * @since v0.0.1
		 */
		void setViewFustrum(ViewFustrum const& newViewFustrum) {
			mViewFustrum = newViewFustrum;
		}

		/**
		 * @brief Sets the projection type.
		 * 
		 * @param newProjectionType The new projection type
		 * 
		 * @since v0.0.1
		 */
		void setProjectionType(ECameraProjectionType newProjectionType) {
			mProjectionType = newProjectionType;
		}

		/**
		 * @brief Sets the camera mode.
		 * 
		 * @param newCameraMode The new camera mode
		 * 
		 * @since v0.0.1
		 */
		void setCameraMode(ECameraPOV newCameraMode) {
			mCameraPOV = newCameraMode;
		}

		/**
		 * @brief Sets the world position.
		 * 
		 * @param newWorldPosition The new world position
		 * 
		 * @since v0.0.1
		 */
		void setWorldPosition(Vector3 const& newWorldPosition) {
			mWorldPosition = newWorldPosition;
		}

		/**
		 * @brief Sets the view matrix.
		 * 
		 * @param newViewMatrix The new view matrix
		 * 
		 * @since v0.0.1
		 */
		void setViewMatrix(Matrix4 const& newViewMatrix) {
			mViewMatrix = newViewMatrix;
		}

		/**
		 * @brief Sets the projection matrix.
		 * 
		 * @param newProjectionMatrix The new projection matrix
		 * 
		 * @since v0.0.1
		 */
		void setProjectionMatrix(Matrix4 const& newProjectionMatrix) {
			mProjectionMatrix = newProjectionMatrix;
		}

		/**
		 * @brief Checks if the view matrix is dirty.
		 * 
		 * @return @c True if the view matrix is dirty, @c false otherwise.
		 * 
		 * @since v0.0.1
		 */
		bool isViewDirty() const { 
			return mbIsViewDirty;
		}

		/**
		 * @brief Gets the field of view.
		 * 
		 * @return The field of view.
		 * 
		 * @since v0.0.1
		 */
		float getFOV() const { 
			return mFOV;
		}

		/**
		 * @brief Gets the aspect ratio.
		 * 
		 * @return The aspect ratio.
		 * 
		 * @since v0.0.1
		 */
		float getAspectRatio() const {
			return mAspectRatio;
		}

		/**
		 * @brief Gets the view fustrum.
		 * 
		 * @return The view fustrum.
		 * 
		 * @since v0.0.1
		 */
		const ViewFustrum& getViewFustrum() const {
			return mViewFustrum;
		}

		/**
		 * @brief Gets the projection type.
		 * 
		 * @return The projection type.
		 * 
		 * @since v0.0.1
		 */
		ECameraProjectionType getProjectionType() const {
			return mProjectionType;
		}

		/**
		 * @brief Gets the camera mode.
		 * 
		 * @return The camera mode.
		 * 
		 * @since v0.0.1
		 */
		ECameraPOV getCameraMode() const {
			return mCameraPOV;
		}

		/**
		 * @brief Gets the world position.
		 * 
		 * @return The world position.
		 * 
		 * @since v0.0.1
		 */
		Vector3 getWorldPosition() const {
			return mWorldPosition;
		}

		/**
		 * @brief Gets the view matrix.
		 * 
		 * @return The view matrix.
		 * 
		 * @since v0.0.1
		 */
		const Matrix4& getViewMatrix() const {
			return mViewMatrix;
		}

		/**
		 * @brief Gets the projection matrix.
		 * 
		 * @return The projection matrix.
		 * 
		 * @since v0.0.1
		 */
		const Matrix4& getProjectionMatrix() const { 
			return mProjectionMatrix;
		}
	};

	class ICameraController {
	public:
		

	};
}
