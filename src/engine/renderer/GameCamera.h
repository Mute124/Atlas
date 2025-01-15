<<<<<<< HEAD
#pragma once
#include "../math/Vector.h"
#include <raylib.h>
#include <cstdint>
#include <unordered_map>
<<<<<<< HEAD
namespace Techstorm {
=======
/**
* \file GameCamera.h
* \brief Declares the GameCamera class
*/
#pragma once
#include "../math/Vector.h"
#include <raylib.h>
namespace Techstorm {
#ifdef TS_RENDERER_2D
	struct CameraData {
		// TODO: Implement 2D camera
	};
#else // TS_RENDERER_3D
	struct CameraData {
		Vec3 position;
		Vec3 target;
		Vec3 up;
		float fovy;
		int projection;
	};
#endif

	class Renderer;

>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======

namespace Atlas {
>>>>>>> main
	/// <summary>
	/// Represents a camera wherein the world is rendered and viewed from.
	/// </summary>
	class GameCamera {
	public:
<<<<<<< HEAD
<<<<<<< HEAD
		GameCamera() {
			init();


		}
=======
		GameCamera();
>>>>>>> main
		
		/// <summary>
		/// Finalizes an instance of the <see cref="GameCamera"/> class. Please note that if this is the main camera, it will make the camera with ID 0 the main camera.
		/// </summary>
		virtual ~GameCamera();

		/// <summary>
		/// Initializes this instance.
		/// </summary>
		void init();
=======
		GameCamera() = default;
		~GameCamera() = default;
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc

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
		Camera3D getCameraData();

		/// <summary>
		/// Gets the position.
		/// </summary>
		/// <returns></returns>
		Vector3 getPosition() const;

		/// <summary>
		/// Gets the target.
		/// </summary>
		/// <returns></returns>
		Vector3 getTarget() const;

		/// <summary>
		/// Gets up.
		/// </summary>
		/// <returns></returns>
		Vector3 getUp() const;

		/// <summary>
		/// Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
		/// </summary>
		/// <returns>The camera's fovy</returns>
		/// \ref how-cameras-work
		float getFovy() const;

		/// <summary>
		/// Gets the camera's projection.
		/// </summary>
		/// <returns></returns>
		int getProjection() const;
<<<<<<< HEAD
		
		static inline GameCamera* GetMainCamera() { return GameCamera::sMainCamera; }

=======
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc

		/// <summary>
		/// Sets the camera data.
		/// </summary>
		/// <param name="data">The data.</param>
		void setCameraData(Camera3D const& data);

		/// <summary>
		/// Sets the position.
		/// </summary>
		/// <param name="position">The position.</param>
		void setPosition(Vector3 position);

		/// <summary>
		/// Sets the target.
		/// </summary>
		/// <param name="target">The target.</param>
		void setTarget(Vector3 target);

		/// <summary>
		/// Sets up.
		/// </summary>
		/// <param name="up">Up.</param>
		void setUp(Vector3 up);

		/// <summary>
		/// Sets the fovy.
		/// </summary>
		/// <param name="fovy">The fovy.</param>
		void setFovy(float fovy);

		/// <summary>
		/// Sets the projection.
		/// </summary>
		/// <param name="projection">The projection.</param>
		void setProjection(int projection);
<<<<<<< HEAD
		
		/// <summary>
		/// Sets this instance as the main camera.
		/// </summary>
		void setAsMainCamera() { 
			sMainCamera = this;
			mIsMainCamera = true; 
		}

		/// <summary>
		/// Checks if this instance is the main camera.
		/// </summary>
		/// <returns></returns>
		bool isMainCamera() const { return mIsMainCamera; }

		bool operator==(const GameCamera& other) const = default;

		Camera3D mCameraData = { 0 };
	private:
		static inline uint16_t sCameraCount = 0;
		static inline std::unordered_map<uint16_t, GameCamera*> sCameras;
		static inline GameCamera* sMainCamera = nullptr;

		uint16_t mCameraID = 0;
		bool mIsMainCamera = false;

		static inline void SimpleCameraUpdate(GameCamera const& cam) {}
		static inline void ComplexCameraUpdate(GameCamera const& cam) {}
=======

	private:
		Camera3D mCameraData;

		static inline void SimpleCameraUpdate(GameCamera& cam) {}
		static inline void ComplexCameraUpdate(GameCamera& cam) {}
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc

		friend class Renderer; /// \ref Renderer
	};
}
