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
	/**
	 * @brief Represents a camera wherein the world is rendered and viewed from.
	 * @todo Add 2D camera support.
	 * @since v0.0.8
	 */
	class GameCamera {
	private:
				
		/**
		 * @brief Directional input
		 * @since v0.0.8
		 * @note This is currently not used, but there is plans for it to be used in the future, hence it is still here and not deprecated.
		 * @todo Implement this in mDirectionalInput. 
		 */
		class DirectionalInput final {
		private:			

			/**
			 * @brief the underlying value that is associated with this direction.
			 * @since v0.0.8
			 */
			float mValue = 0.0f;

			/**
			 * @brief The mutex that ensures that this input is thread safe.
			 * @since v0.0.8
			 */
			std::mutex mMutex;
		public:			

			
			/**
			 * @brief Initializes a new instance of the DirectionalInput class.
			 * @param value The floating point value to associate with this input.
			 * @since v0.0.8
			 */
			DirectionalInput(float value) : mValue(value) {}
			
			/**
			 * @brief Initializes a new instance of the DirectionalInput class.
			 * @remark This is the default constructor.
			 * @since v0.0.8
			 */
			DirectionalInput() = default;

			/**
			 * @brief Resets the value associated with this input.
			 * @note This is thread safe and locks the mutex.
			 * @since v0.0.8
			 */
			void reset() {
				std::scoped_lock<std::mutex> lock(mMutex);
				mValue = 0.0f;
			}

			/**
			 * @brief Sets mValue to the specified floating point value.
			 * @param value What float to set mValue to.
			 * @note This is thread safe and locks the mutex.
			 * @since v0.0.8
			 */
			void set(float value) {
				std::scoped_lock<std::mutex> lock(mMutex);
				mValue = value;
			}
			
			/**
			 * @brief Adds mValue with the specified value.
			 * @param value the delta to add mValue with.
			 * @note This is thread safe and locks the mutex.
			 * @since v0.0.8
			 */
			void add(float value) {
				std::scoped_lock<std::mutex> lock(mMutex);
				mValue += value;
			}
			
			/**
			 * @brief Gets the value.
			 * @remarks This only returns a float, not a reference.
			 * @return The current value of mValue.
			 * @since v0.0.8
			 */
			float getValue() const { return mValue; }

			/**
			 * @brief Implements the operator float.
			 * @returns The result of the operator.
			 * @since v0.0.8
			 */
			operator float() const { return mValue; }
			
			/**
			 * @brief This operator sets mValue to the value parameter @ref value.
			 * @remarks This is thread safe and locks the mutex.
			 * @param value The value to SET mValue to.
			 * @return A reference to this object after setting mValue.
			 * @since v0.0.8
			 */
			DirectionalInput& operator=(float value) {
				set(value);
				return *this;
			}
			
			/**
			 * @brief This just increments mValue by the value parameter @ref value. 
			 * @remarks This is thread safe and locks the mutex. Additionally, it returns a reference to this object after it has ADDED to mValue.
			 * In other words, this works the same way as the @ref add function
			 * @param value The value to ADD to mValue.
			 * @return 
			 * @since v0.0.8
			 */
			DirectionalInput& operator+=(float value) {
				add(value);
				return *this;
			}
		};
		
		/**
		 * @brief A uint16_t that represents the number of cameras that have been created.
		 * @remarks This is static inline and is incremented every time a camera is created. This is also used to create the camera identifiers before
		 * they are put in @ref sCameras.
		 * @since v0.0.3
		 */
		static inline uint16_t sCameraCount = 0;
		
		/**
		 * @brief This unordered map contains all of the cameras that have been created.
		 * @remarks This is static inline and is used to store all of the cameras that have been created. The uint16_t represents the camera's
		 * identifier. The reason for why it is a uint16_t is because it is used as a key in the map and there does not need to be more than 65535 
		 * cameras.
		 * @note The GameCamera is stored as a pointer to the camera.
		 * @since v0.0.3
		 */
		static inline std::unordered_map<uint16_t, GameCamera*> sCameras;

		/**
		 * @brief This is a pointer to what ever camera is currently the main camera.
		 * @since v0.0.3
		 */
		static inline GameCamera* sMainCamera = nullptr;
		
		/**
		 * @brief This camera's unique identifier in the @ref sCameras map.
		 * @remarks this is a uint16_t that is used as a key in the @ref sCameras map.
		 * @since v0.0.3
		 */
		uint16_t mCameraID = 0;

		/**
		 * @brief A flag that indicates if this camera is the main camera.
		 * @remarks This is set to true if this camera is the main camera. If this is the main camera, then it will set the @ref sMainCamera.
		 * This can happen in 
		 * @since v0.0.3
		 */
		bool mIsMainCamera = false;

		/**
		 * @remarks The Renderer class is being friended here because it requires access to this camera's updating and other functions.
		 * @since v0.0.3
		 */
		friend class Renderer; 
	protected:
		
		/**
		 * @remarks The CameraInputAction class (@see CameraInputAction) is being friended here because it requires access to this
		 * camera's @ref mDirectionalInput.
		 * @since v0.0.8
		 */
		friend class CameraInputAction;

		/**
		 * @brief 
		 * @remarks This enum is used to represent the different directions that the camera can move. This is used in 
		 * the @ref mDirectionalInput array and is used to access 
		 * @ref mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::FORWARD_DIRECTION)] within the @ref GameCamera class.
		 * It is also used within the @ref CameraInputAction class.
		 * @since v0.0.8
		 */
		enum class EMovementDirection {

			/**
			 * @brief Represents the forward direction. 
			 * @remarks This is used to move the camera forward.
			 * @since v0.0.8
			 */
			FORWARD_DIRECTION, 

			/**
			 * @brief Represents the backward direction. 
			 * @remarks This is used to move the camera backward.
			 * @since v0.0.8
			 */
			BACKWARD_DIRECTION,

			/**
			 * @brief Represents the left direction. 
			 * @remarks This is used to move the camera left.
			 * @since v0.0.8
			 */
			LEFT_DIRECTION,

			/**
			 * @brief Represents the right direction. 
			 * @remarks This is used to move the camera right.
			 * @since v0.0.8
			 */
			RIGHT_DIRECTION,

			/**
			 * @brief Represents the up direction. 
			 * @remarks This is used to move the camera up.
			 * @since v0.0.8
			 */
			UP_DIRECTION,

			/**
			 * @brief Represents the down direction. 
			 * @remarks This is used to move the camera down.
			 * @since v0.0.8
			 */
			DOWN_DIRECTION
		};		

		/**
		 * @brief Like an InputAction, this class is used to handle input for the camera. 
		 * @since v0.0.8
		 */
		class CameraInputAction : public InputAction {
		public:			

			/**
			 * @brief A pointer to the camera that this input action is associated with.
			 */
			GameCamera* camera = nullptr;

			/**
			 * @brief Initializes a new instance of the <see cref="CameraInputAction"/> class.
			 * @param name The name of the input action.
			 * @param key The key that is associated with the input action.
			 * @param type The type of the input action.
			 * @param trigger The trigger that is associated with the input action.
			 * @param camera The camera that is associated with the input action.
			 * @since v0.0.8
			 */
			CameraInputAction(const std::string& name, int key, const EInputType& type, const EInputTrigger& trigger, GameCamera* camera);
			
			/**
			 * @brief Move the camera forward.
			 * @param amount The amount to move the camera forward.
			 * @since v0.0.8
			 */
			void moveForward(float amount);
			
			/**
			 * @brief Move the camera backward.
			 * @param amount The amount to move the camera backward.
			 * @since v0.0.8
			 */
			void moveBackward(float amount);
			
			/**
			 * @brief Move the camera left.
			 * @param amount The amount to move the camera left.
			 * @since v0.0.8
			 */
			void moveLeft(float amount);
			
			/**
			 * @brief Move the camera right.
			 * @param amount The amount to move the camera right.
			 * @since v0.0.8
			 */
			void moveRight(float amount);
			
			/**
			 * @brief Move the camera up.
			 * @param amount The amount to move the camera up.
			 * @since v0.0.8
			 */
			void moveUp(float amount);
			
			/**
			 * @brief Move the camera down.
			 * @param amount The amount to move the camera down.
			 * @since v0.0.8
			 */
			void moveDown(float amount);
		};
				
		/**
		 * @brief This is how the CameraInputAction class is used to handle input for the camera.
		 * @remarks The CameraInputAction class is used to handle input for the camera. Each CameraInputAction is assigned a direction. Then that
		 * direction is used to move the camera in that direction. The @ref EMovementDirection enum is used to represent the directions, aka the
		 * index within this array. To see this in action, please see the @ref how-cameras-work and @ref GameCamera in the documentation.
		 * @since v0.0.8
		 */
		std::array<float, 6> mDirectionalInput = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	public:
		
		/**
		 * @brief Initializes a new instance of the GameCamera class.
		 * @note This is the default constructor and does not do anything.
		 * @since v0.0.3
		 */
		GameCamera();
		
		/**
		 * @brief Finalizes an instance of the GameCamera class.
		 * @note Please note that if this is the main camera, it will make the camera with ID 0 the main camera.
		 * @since v0.0.3
		 */
		virtual ~GameCamera();

		/**
		 * @brief Initializes this instance.
		 * @since v0.0.3
		 */
		virtual void init();		

		/**
		 * @brief Updates this instance in one of two ways, depending on the camera's settings.
		 * @remarks For a more in-depth explanation of camera settings, see @ref how-cameras-work and @ref GameCamera in the documentation.
		 * @since v0.0.3
		 */
		virtual void update();

		/**
		 * @brief Gets the underlying mCameraData struct. 
		 * @remarks The mCameraData struct is used to store the camera's data.
		 * @note The mCameraData struct type is Raylib's Camera3D type.
		 * @return 
		 */
		virtual Camera3D getCameraData();

		/**
		 * @brief Returns the current position of this camera
		 * @return A Vector3 that represents the current position
		 * @since v0.0.3
		 */
		virtual Vector3 getPosition() const;

		/**
		 * @brief Returns the current target of this camera. 
		 * @remarks In other words, this returns the position that the camera is looking at
		 * @return A Vector3 that represents the current target
		 * @since v0.0.3
		 */
		virtual Vector3 getTarget() const;

		/**
		 * @brief Returns the current up vector of this camera.
		 * @return A Vector3 that represents the current up vector
		 * @since v0.0.3
		 */
		virtual Vector3 getUp() const;

		/**
		 * @brief Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
		 * @see @ref how-cameras-work
		 * @return The camera's fovy (Field of View Y)
		 * @since v0.0.3
		 */
		virtual float getFovy() const;

/**
		 * @brief Gets the camera's projection.
		 * @see @ref how-cameras-work
		 * @return The camera's projection. This is a Raylib enum
		 * @since v0.0.3
		 */
		virtual int getProjection() const;
				
		/**
		 * @brief This function is a static function and is used to get the main camera.
		 * @return The main camera
		 * @since v0.0.3
		 */
		static inline GameCamera* GetMainCamera();

		/**
		 * @brief Sets this camera's underlying Camera3D struct.
		 * @param data A const reference to a Camera3D struct.
		 * @note The parameter is a const reference to a Camera3D struct, but the function just copies the struct to the mCameraData struct.
		 * @since v0.0.3
		 */
		virtual void setCameraData(Camera3D const& data);

		/**
		 * @brief Sets this camera's position
		 * @param position The position that this camera should be at
		 * @since v0.0.3
		 */
		virtual void setPosition(Vector3 position);

		
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
