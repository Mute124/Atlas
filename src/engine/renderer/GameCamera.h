/*****************************************************************//**
 * @file   GameCamera.h
 * 
 * @brief  Contains everything that is related to the GameCamera class.
 * 
 * @date   September 2024
 * 
 * @since v0.0.3
 *********************************************************************/
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <raylib.h>

#include "../input/Input.h"
#include "../math/Vector.h"

/**
 * @brief Defines the total number of directional inputs that
 * the camera supports. The reason for only 6 directions
 * is because that is the amount of directions that a 3D
 * camera can move in. These are the 6 directions:
 * - Forward
 * - Backward
 * - Right
 * - Left
 * - Up
 * - Down
 * 
 * @since v0.0.9
 */
#define ATLAS_DIRECTIONAL_INPUT_COUNT 6

namespace Atlas {

	using CameraCounterType = uint16_t;


	template<typename T_CAMERA_DATA_TYPE>
	class IGameCamera {
	public:
		/**
		 * @brief Initializes this instance.
		 *
		 * @since v0.0.3
		 */
		virtual void init() = 0;

		/**
		 * @brief Updates this instance in one of two ways, depending on the camera's settings.
		 *
		 * @remarks For a more in-depth explanation of camera settings, see @ref how-cameras-work and @ref GameCamera in the documentation.
		 *
		 * @since v0.0.3
		 */
		virtual void update() = 0;

		/**
		 * @brief Gets the underlying mCameraData struct.
		 *
		 * @remarks The mCameraData struct is used to store the camera's data.
		 *
		 * @note The mCameraData struct type is Raylib's Camera3D type.
		 *
		 * @return
		 */
		virtual T_CAMERA_DATA_TYPE getCameraData() = 0;

		/**
		 * @brief Returns the current position of this camera
		 *
		 * @return A Vector3 that represents the current position
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual Vector3 getPosition() const = 0;

		/**
		 * @brief Returns the current target of this camera.
		 *
		 * @remarks In other words, this returns the position that the camera is looking at
		 *
		 * @return A Vector3 that represents the current target
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual Vector3 getTarget() const = 0;

		/**
		 * @brief Returns the current up vector of this camera.
		 *
		 * @return A Vector3 that represents the current up vector
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual Vector3 getUp() const = 0;

		/**
		 * @brief Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
		 *
		 * @return The camera's fovy (Field of View Y)
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual float getFovy() const = 0;

		/**
		 * @brief Gets the camera's projection.
		 *
		 * @return The camera's projection. This is a Raylib enum
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual int getProjection() const = 0;

		/**
		 * @brief This function is a static function and is used to get the main camera.
		 *
		 * @return The main camera
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		static inline T_CAMERA_DATA_TYPE* GetMainCamera() = 0;

		/**
		 * @brief Sets this camera's underlying Camera3D struct.
		 *
		 * @note The parameter is a const reference to a Camera3D struct, but the function just copies the struct to the mCameraData struct.
		 *
		 * @param data A const reference to a Camera3D struct.
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual void setCameraData(T_CAMERA_DATA_TYPE const& data) = 0;

		/**
		 * @brief Sets this camera's position
		 *
		 * @param position The position that this camera should be at
		 *
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual void setPosition(Vector3 position) = 0;


		virtual void setTarget(Vector3 target) = 0;

		/// <summary>
		/// Sets the camera's up vector. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <param name="up">Up.</param>
		virtual void setUp(Vector3 up) = 0;

		/// <summary>
		/// Sets the fovy. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <param name="fovy">The fovy.</param>
		virtual void setFovy(float fovy) = 0;

		/// <summary>
		/// Sets the projection. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <param name="projection">The projection.</param>
		virtual void setProjection(int projection) = 0;

		/// <summary>
		/// Sets this instance as the main camera. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		virtual void setAsMainCamera() = 0;

		/// <summary>
		/// Checks if this instance is the main camera. This function is virtual because it can be overridden by derived classes.
		/// </summary>
		/// <returns></returns>
		virtual bool isMainCamera() const = 0;
	};

	template<uint16_t T_MAX_CAMERAS, class T_BASE_CAMERA_TYPE>
	class ICameraRegistry {
	protected:
		CameraCounterType mMaxCameras = T_MAX_CAMERAS;
	public:

		virtual bool isMainCamera(T_BASE_CAMERA_TYPE camera) = 0;
		virtual void setMainCamera(T_BASE_CAMERA_TYPE camera) = 0;
		virtual T_BASE_CAMERA_TYPE getMainCamera() = 0;

		virtual uint16_t getTotalCameraCount() = 0;
	};

	template<uint16_t T_MAX_CAMERAS, class T_BASE_CAMERA_TYPE>
	class ICameraPointerRegistry : public ICameraRegistry<T_MAX_CAMERAS, T_BASE_CAMERA_TYPE*>
	{
		// Left empty as this is still a pure virtual class of ICameraRegistry.
	};

	/**
	 * @brief Represents a camera wherein the world is rendered and viewed
	 * from.
	 * 
	 * @todo Add 2D camera support.
	 * 
	 * @since v0.0.3
	 */
	class GameCamera {
	private:
		/**
		 * @remarks The Renderer class is being friended here because it
		 * requires access to this camera's updating and other functions.
		 *
		 * @since v0.0.3
		 */
		friend class Renderer;

		/**
		 * @brief A uint16_t that represents the total number of
		 * cameras that have been created. This is used to create
		 * the camera identifiers before they are put in sCameras.
		 * 
		 * @since v0.0.3
		 */
		static inline uint16_t sCameraCount = 0;
		
		/**
		 * @brief A static inline unordered map that stores all of
		 * the cameras that have been created within the world. The
		 * uint16_t represents the camera's identifier, which is used
		 * as a key in the map (aka an ID). 
		 * 
		 * @remarks The reason for why it is a uint16_t is because it
		 * is used as a key in the map and there does not need to be more
		 * than 65535 cameras in the world. If you need more than 65535
		 * cameras, may I ask why on god's green earth you need so many?
		 * 
		 * @since v0.0.3
		 */
		static inline std::unordered_map<uint16_t, GameCamera*> sCameras;

		/**
		 * @brief This is a pointer to what ever camera is currently the
		 * main camera.
		 * 
		 * @since v0.0.3
		 */
		static inline GameCamera* sMainCamera = nullptr;
		
		/**
		* @brief The raylib camera data struct that is used to interface with raylib.
		*
		* @since v0.0.3
		*/
		Camera3D mCameraData = { 0 };

		/**
		 * @brief This camera's unique identifier in the @ref sCameras map.
		 * 
		 * @remarks this is a uint16_t that is used as a key in the @ref sCameras map.
		 * 
		 * @since v0.0.3
		 */
		uint16_t mCameraID = 0;

		/**
		 * @brief A flag that indicates if this camera is the main camera.
		 * 
		 * @remarks This is set to true if this camera is the main camera.
		 * If this is the main camera, then it will set the @ref sMainCamera.
		 * This can happen in 
		 * 
		 * @since v0.0.3
		 */
		bool mIsMainCamera = false;

	protected:
		
		/**
		 * @remarks The CameraInputAction class is being friended here
		 * because it requires access to this camera's mDirectionalInput.
		 * 
		 * @since v0.0.8
		 * 
		 * @sa @ref CameraInputAction for the class being friended
		 */
		friend class CameraInputAction;

		/**
		 * @brief This using statement is used to help with the mDirectionalInput
		 * array (see the see also section within this doxycomment). 
		 * 
		 * @note The reason for this being a using declaration instead of
		 * simply using the std::array is for further type strength.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref mDirectionalInput for where this is used. 
		 * 
		 * @sa @ref ATLAS_DIRECTIONAL_INPUT_COUNT for the macro that defines
		 * how many directional inputs there are.
		 */
		using DirectionalInputArray = std::array<float, ATLAS_DIRECTIONAL_INPUT_COUNT>;

		/**
		 * @brief A enumerated representaiton of different directions that
		 * are used in the mDirectionalInput array. The values of each
		 * enum are equivalent to the index in the mDirectionalInput array.
		 * 
		 * @remarks This enum is used to represent the different directions
		 * that the camera can move. This is used in the @ref mDirectionalInput
		 * array to access the directional inputs. An example of this is 
		 * below:
		 * @code
		 * mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::FORWARD_DIRECTION)] 
		 * @endcode 
		 * However, this is really only used within the @ref CameraInputAction
		 * and the @ref GameCamera class. 
		 * 
		 * @since v0.0.8
		 */
		enum class EMovementDirection {

			/**
			 * @brief Represents the forward direction. 
			 * 
			 * @since v0.0.8
			 */
			FORWARD_DIRECTION, 

			/**
			 * @brief Represents the backward direction. 
			 * 
			 * @since v0.0.8
			 */
			BACKWARD_DIRECTION,

			/**
			 * @brief Represents the left direction. 
			 *
			 * @since v0.0.8
			 */
			LEFT_DIRECTION,

			/**
			 * @brief Represents the right direction. 
			 * 
			 * @since v0.0.8
			 */
			RIGHT_DIRECTION,

			/**
			 * @brief Represents the up direction. 
			 *
			 * @since v0.0.8
			 */
			UP_DIRECTION,

			/**
			 * @brief Represents the down direction. 
			 *
			 * @since v0.0.8
			 */
			DOWN_DIRECTION
		};		

		/**
		 * @brief Like an InputAction, this class is used to handle input
		 * for the camera. 
		 * 
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
		 * @brief This is how the CameraInputAction class is used to handle
		 * input for the camera.
		 * 
		 * @remarks The CameraInputAction class is used to handle input for
		 * the camera. Each CameraInputAction is assigned a direction. Then
		 * that direction is used to move the camera in that direction. The
		 * @ref EMovementDirection enum is used to represent the directions, 
		 * aka the index within this array. To see this in action, please see
		 * the @ref how-cameras-work and @ref GameCamera in the documentation.
		 * 
		 * @since v0.0.8
		 */
		DirectionalInputArray mDirectionalInput = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

		/**
		 * @brief Converts a direction (as EMovementDirection) to an
		 * index (as size_t) through static_cast. 
		 * 
		 * @remarks The value it returns is the index within the 
		 * @ref mDirectionalInput array.
		 * 
		 * @param direction A const reference to the direction to convert.
		 * 
		 * @return An index within the mDirectionalInput array.
		 * 
		 * @since v0.0.9
		 */
		static inline size_t GetDirectionalInputAsIndex(const EMovementDirection& direction);

	public:

		/**
		 * @brief Initializes a new instance of the GameCamera class.
		 * 
		 * @note This is the default constructor and does not do anything.
		 * 
		 * @since v0.0.3
		 */
		GameCamera();
		
		/**
		 * @brief Finalizes an instance of the GameCamera class.
		 * 
		 * @note Please note that if this is the main camera, it will make
		 * the camera with ID 0 the main camera.
		 * 
		 * @since v0.0.3
		 */
		virtual ~GameCamera();

		/**
		 * @brief Initializes this instance.
		 * 
		 * @since v0.0.3
		 */
		virtual void init();		

		/**
		 * @brief Updates this instance in one of two ways, depending on the
		 * camera's settings.
		 * 
		 * @remarks For a more in-depth explanation of camera settings, see
		 * @ref how-cameras-work and @ref GameCamera in the documentation.
		 * 
		 * @since v0.0.3
		 */
		virtual void update();

		/**
		 * @brief Gets the underlying mCameraData struct. 
		 * 
		 * @remarks The mCameraData struct is used to store the camera's data.
		 * 
		 * @note The mCameraData struct type is Raylib's Camera3D type.
		 * 
		 * @return 
		 */
		virtual Camera3D getCameraData();

		/**
		 * @brief Returns the current position of this camera
		 * 
		 * @return A Vector3 that represents the current position
		 * 
		 * @since v0.0.3
		 * 
		 * @sa @ref how-cameras-work
		 */
		virtual Vector3 getPosition() const;

		/**
		 * @brief Returns the current target of this camera. 
		 * 
		 * @remarks In other words, this returns the position that the camera
		 * is looking at
		 * 
		 * @return A Vector3 that represents the current target
		 * 
		 * @since v0.0.3
		 * 
		 * @sa @ref how-cameras-work
		 */
		virtual Vector3 getTarget() const;

		/**
		 * @brief Returns the current up vector of this camera.
		 * 
		 * @return A Vector3 that represents the current up vector
		 * 
		 * @since v0.0.3
		 * 
		 * @sa @ref how-cameras-work
		 */
		virtual Vector3 getUp() const;

		/**
		 * @brief Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
		 * 
		 * @return The camera's fovy (Field of View Y)
		 * 
		 * @since v0.0.3
		 * 
		 * @sa @ref how-cameras-work
		 */
		virtual float getFovy() const;

		/**
		 * @brief Gets the camera's projection.
		 * 
		 * @return The camera's projection. This is a Raylib enum
		 * 
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		virtual int getProjection() const;
				
		/**
		 * @brief This function is a static function and is used to get the main camera.
		 * 
		 * @return The main camera
		 * 
		 * @since v0.0.3
		 *
		 * @sa @ref how-cameras-work
		 */
		static inline GameCamera* GetMainCamera();

		/**
		 * @brief Sets this camera's underlying Camera3D struct.
		 *
		 * @note The parameter is a const reference to a Camera3D struct, but the function just copies the struct to the mCameraData struct.
		 * 
		 * @param data A const reference to a Camera3D struct.	
		 * 
		 * @since v0.0.3
		 * 
		 * @sa @ref how-cameras-work
		 */
		virtual void setCameraData(Camera3D const& data);

		/**
		 * @brief Sets this camera's position
		 * 
		 * @param position The position that this camera should be at
		 * 
		 * @since v0.0.3
		 * 
		 * @sa @ref how-cameras-work
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
		
		/**
		 * @brief The @b default equality operator. This is a const function.
		 * 
		 * @param other A const reference to another GameCamera
		 * 
		 * @return Whether or not both values equal one another
		 * 
		 * @since v0.0.3
		 */
		bool operator==(const GameCamera& other) const = default;
	};
}
