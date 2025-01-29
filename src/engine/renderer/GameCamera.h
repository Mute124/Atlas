/**
* @file GameCamera.h
* @todo Work on documentation.
*/
#pragma once
#include "../math/Vector.h"
#include <raylib.h>
#include <cstdint>
#include <unordered_map>
#include <array>
#include <memory>
#include <mutex>

#include "../input/Input.h"

namespace Atlas {


	/// <summary>
	/// Represents a camera wherein the world is rendered and viewed from.
	/// </summary>
	class GameCamera {
	private:
		
		/// <summary>
		/// 
		/// </summary>
		class DirectionalInput final {
		private:			
			/// <summary>
			/// The m value
			/// </summary>
			float mValue = 0.0f;

			/// <summary>
			/// The m mutex
			/// </summary>
			std::mutex mMutex;


		public:			
			/// <summary>
			/// Initializes a new instance of the <see cref="DirectionalInput"/> class.
			/// </summary>
			/// <param name="value">The value.</param>
			DirectionalInput(float value) : mValue(value) {}

			/// <summary>
			/// Initializes a new instance of the <see cref="DirectionalInput"/> class.
			/// </summary>
			DirectionalInput() = default;
			
			/// <summary>
			/// Resets this instance.
			/// </summary>
			void reset() {
				std::scoped_lock<std::mutex> lock(mMutex);
				mValue = 0.0f;
			}
			
			/// <summary>
			/// Sets the specified value.
			/// </summary>
			/// <param name="value">The value.</param>
			void set(float value) {
				std::scoped_lock<std::mutex> lock(mMutex);
				mValue = value;
			}
			
			/// <summary>
			/// Adds the specified value.
			/// </summary>
			/// <param name="value">The value.</param>
			void add(float value) {
				std::scoped_lock<std::mutex> lock(mMutex);
				mValue += value;
			}
			
			/// <summary>
			/// Gets the value.
			/// </summary>
			/// <returns></returns>
			float getValue() const { return mValue; }
			
			/// <summary>
			/// Implements the operator float.
			/// </summary>
			/// <returns>
			/// The result of the operator.
			/// </returns>
			operator float() const { return mValue; }
						
			/// <summary>
			/// Operator=s the specified value.
			/// </summary>
			/// <param name="value">The value.</param>
			/// <returns></returns>
			DirectionalInput& operator=(float value) {
				set(value);
				return *this;
			}
			
			/// <summary>
			/// Operator+=s the specified value.
			/// </summary>
			/// <param name="value">The value.</param>
			/// <returns></returns>
			DirectionalInput& operator+=(float value) {
				add(value);
				return *this;
			}
		};
		
		/// <summary>
		/// The s camera count
		/// </summary>
		static inline uint16_t sCameraCount = 0;
		
		/// <summary>
		/// The s cameras
		/// </summary>
		static inline std::unordered_map<uint16_t, GameCamera*> sCameras;
		
		/// <summary>
		/// The s main camera
		/// </summary>
		static inline GameCamera* sMainCamera = nullptr;
		
		/// <summary>
		/// The m camera identifier
		/// </summary>
		uint16_t mCameraID = 0;

		/// <summary>
		/// The m is main camera
		/// </summary>
		bool mIsMainCamera = false;

		friend class Renderer; /// \ref Renderer
	protected:
		
		friend class CameraInputAction;


		/// <summary>
		/// 
		/// </summary>
		enum class EMovementDirection {
			FORWARD_DIRECTION,
			BACKWARD_DIRECTION,
			LEFT_DIRECTION,
			RIGHT_DIRECTION,
			UP_DIRECTION,
			DOWN_DIRECTION
		};		

		/// <summary>
		/// 
		/// </summary>
		/// <seealso cref="InputAction" />
		class CameraInputAction : public InputAction {
		public:			
			/// <summary>
			/// The camera
			/// </summary>
			GameCamera* camera = nullptr;
			
			/// <summary>
			/// Initializes a new instance of the <see cref="CameraInputAction"/> class.
			/// </summary>
			/// <param name="name">The name.</param>
			/// <param name="key">The key.</param>
			/// <param name="type">The type.</param>
			/// <param name="trigger">The trigger.</param>
			/// <param name="camera">The camera.</param>
			CameraInputAction(const std::string& name, int key, const EInputType& type, const EInputTrigger& trigger, GameCamera* camera);
			
			/// <summary>
			/// Moves the forward.
			/// </summary>
			/// <param name="amount">The amount.</param>
			void moveForward(float amount);
			
			/// <summary>
			/// Moves the backward.
			/// </summary>
			/// <param name="amount">The amount.</param>
			void moveBackward(float amount);
			
			/// <summary>
			/// Moves the left.
			/// </summary>
			/// <param name="amount">The amount.</param>
			void moveLeft(float amount);
			
			/// <summary>
			/// Moves the right.
			/// </summary>
			/// <param name="amount">The amount.</param>
			void moveRight(float amount);
			
			/// <summary>
			/// Moves up.
			/// </summary>
			/// <param name="amount">The amount.</param>
			void moveUp(float amount);
			
			/// <summary>
			/// Moves down.
			/// </summary>
			/// <param name="amount">The amount.</param>
			void moveDown(float amount);
		};
				
		/// <summary>
		/// The m directional input
		/// </summary>
		std::array<float, 6> mDirectionalInput = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	public:
		
		/// <summary>
		/// Initializes a new instance of the <see cref="GameCamera"/> class.
		/// </summary>
		GameCamera();
		
		/// <summary>
		/// Finalizes an instance of the <see cref="GameCamera"/> class. Please note that if this is the main camera, it will make the camera with ID 0 the main camera.
		/// </summary>
		virtual ~GameCamera();

		/// <summary>
		/// Initializes this instance.
		/// </summary>
		virtual void init();

		/// <summary>
		/// Updates this instance in one of two ways, depending on the camera's settings.
		/// </summary>
		/// <remarks>
		///		For a more in-depth explanation of camera settings, see \ref how-cameras-work and \ref GameCamera in the documentation.
		/// </remarks>
		virtual void update();

		/// <summary>
		/// Gets the camera data.
		/// </summary>
		/// <returns>A reference to the underlying mCameraData variable</returns>
		virtual Camera3D getCameraData();

		/// <summary>
		/// Gets the position.
		/// </summary>
		/// <returns></returns>
		virtual Vector3 getPosition() const;

		/// <summary>
		/// Gets the target.
		/// </summary>
		/// <returns></returns>
		virtual Vector3 getTarget() const;

		/// <summary>
		/// Gets up.
		/// </summary>
		/// <returns></returns>
		virtual Vector3 getUp() const;

		/// <summary>
		/// Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
		/// </summary>
		/// <returns>The camera's fovy</returns>
		/// \ref how-cameras-work
		virtual float getFovy() const;

		/// <summary>
		/// Gets the camera's projection.
		/// </summary>
		/// <returns></returns>
		virtual int getProjection() const;
				
		/// <summary>
		/// Gets the main camera.
		/// </summary>
		/// <returns></returns>
		static inline GameCamera* GetMainCamera();

		/// <summary>
		/// Sets the camera data.
		/// </summary>
		/// <param name="data">The data.</param>
		virtual void setCameraData(Camera3D const& data);

		/// <summary>
		/// Sets the position.
		/// </summary>
		/// <param name="position">The position.</param>
		virtual void setPosition(Vector3 position);

		/// <summary>
		/// Sets the target.
		/// </summary>
		/// <param name="target">The target.</param>
		virtual void setTarget(Vector3 target);

		/// <summary>
		/// Sets the camera's up vector. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <param name="up">Up.</param>
		virtual void setUp(Vector3 up);

		/// <summary>
		/// Sets the fovy. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <param name="fovy">The fovy.</param>
		virtual void setFovy(float fovy);

		/// <summary>
		/// Sets the projection. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <param name="projection">The projection.</param>
		virtual void setProjection(int projection);
		
		/// <summary>
		/// Sets this instance as the main camera. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		virtual void setAsMainCamera();

		/// <summary>
		/// Checks if this instance is the main camera. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <returns></returns>
		virtual bool isMainCamera() const;
		
		/// <summary>
		/// Operator==s the specified other.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		bool operator==(const GameCamera& other) const = default;
		
		/// <summary>
		/// The raylib camera data struct. This is used to interface with raylib.
		/// </summary>
		Camera3D mCameraData = { 0 };
	};
}
