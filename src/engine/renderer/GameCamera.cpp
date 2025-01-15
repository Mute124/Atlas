#include "GameCamera.h"
<<<<<<< HEAD
#include <raylib.h>

Atlas::GameCamera::GameCamera() {
	init();
}

/// <summary>
/// Finalizes an instance of the <see cref="GameCamera"/> class. Please note that if this is the main camera, it will make the camera with ID 0 the main camera.
/// </summary>
Atlas::GameCamera::~GameCamera() {
	if(isMainCamera()) {
		sMainCamera = nullptr;

		if(sCameraCount > 0) {
			sCameras[0]->setAsMainCamera();
		}
	}

	sCameras.erase(mCameraID);
	sCameraCount--;
}

void Atlas::GameCamera::init()
{
	if(sCameraCount == 0) {
		this->setAsMainCamera();
	}

	this->mCameraID = sCameraCount++;

	sCameras[this->mCameraID] = this;
}

void Atlas::GameCamera::update()
{
	UpdateCamera(&mCameraData, CAMERA_FIRST_PERSON);

=======

void Techstorm::GameCamera::update()
{
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
}

/// <summary>
/// Gets the camera data.
/// </summary>
/// <returns>A reference to the underlying mCameraData variable</returns>

Camera3D Atlas::GameCamera::getCameraData() { return mCameraData; }

/// <summary>
/// Gets the position.
/// </summary>
/// <returns></returns>

<<<<<<< HEAD
<<<<<<< HEAD
  Vector3 Techstorm::GameCamera::getPosition() const { return mCameraData.position; }
=======
inline Vector3 Techstorm::GameCamera::getPosition() const { return mCameraData.position; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
  Vector3 Atlas::GameCamera::getPosition() const { return mCameraData.position; }
>>>>>>> main

/// <summary>
/// Gets the target.
/// </summary>
/// <returns></returns>

<<<<<<< HEAD
<<<<<<< HEAD
  Vector3 Techstorm::GameCamera::getTarget() const { return mCameraData.target; }
=======
inline Vector3 Techstorm::GameCamera::getTarget() const { return mCameraData.target; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
  Vector3 Atlas::GameCamera::getTarget() const { return mCameraData.target; }
>>>>>>> main

/// <summary>
/// Gets up.
/// </summary>
/// <returns></returns>
<<<<<<< HEAD
<<<<<<< HEAD
  Vector3 Techstorm::GameCamera::getUp() const { return mCameraData.up; }
=======
inline Vector3 Techstorm::GameCamera::getUp() const { return mCameraData.up; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
  Vector3 Atlas::GameCamera::getUp() const { return mCameraData.up; }
>>>>>>> main

/// <summary>
/// Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
/// </summary>
/// <returns>The camera's fovy</returns>
/// \ref how-cameras-work
<<<<<<< HEAD
<<<<<<< HEAD
  float Techstorm::GameCamera::getFovy() const { return mCameraData.fovy; }
=======
inline float Techstorm::GameCamera::getFovy() const { return mCameraData.fovy; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
  float Atlas::GameCamera::getFovy() const { return mCameraData.fovy; }
>>>>>>> main

/// <summary>
/// Gets the camera's projection.
/// </summary>
/// <returns></returns>
<<<<<<< HEAD
<<<<<<< HEAD
  int Techstorm::GameCamera::getProjection() const { return mCameraData.projection; }
=======
inline int Techstorm::GameCamera::getProjection() const { return mCameraData.projection; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
  int Atlas::GameCamera::getProjection() const { return mCameraData.projection; }
>>>>>>> main

/// <summary>
/// Sets the camera data.
/// </summary>
/// <param name="data">The data.</param>
<<<<<<< HEAD
<<<<<<< HEAD
  void Techstorm::GameCamera::setCameraData(Camera3D const& data) { mCameraData = data; }
=======
inline void Techstorm::GameCamera::setCameraData(Camera3D const& data) { mCameraData = data; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
  void Atlas::GameCamera::setCameraData(Camera3D const& data) { mCameraData = data; }
>>>>>>> main

/// <summary>
/// Sets the position.
/// </summary>
/// <param name="position">The position.</param>
<<<<<<< HEAD
<<<<<<< HEAD
void Techstorm::GameCamera::setPosition(Vector3 position) { mCameraData.position = position; }
=======
inline void Techstorm::GameCamera::setPosition(Vector3 position) { mCameraData.position = position; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
void Atlas::GameCamera::setPosition(Vector3 position) { mCameraData.position = position; }
>>>>>>> main

/// <summary>
/// Sets the target.
/// </summary>
/// <param name="target">The target.</param>
<<<<<<< HEAD
<<<<<<< HEAD
void Techstorm::GameCamera::setTarget(Vector3 target) { mCameraData.target = target; }
=======
inline void Techstorm::GameCamera::setTarget(Vector3 target) { mCameraData.target = target; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
void Atlas::GameCamera::setTarget(Vector3 target) { mCameraData.target = target; }
>>>>>>> main

/// <summary>
/// Sets up.
/// </summary>
/// <param name="up">Up.</param>
<<<<<<< HEAD
<<<<<<< HEAD
void Techstorm::GameCamera::setUp(Vector3 up) { mCameraData.up = up; }
=======
inline void Techstorm::GameCamera::setUp(Vector3 up) { mCameraData.up = up; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
void Atlas::GameCamera::setUp(Vector3 up) { mCameraData.up = up; }
>>>>>>> main

/// <summary>
/// Sets the fovy.
/// </summary>
/// <param name="fovy">The fovy.</param>
<<<<<<< HEAD
<<<<<<< HEAD
void Techstorm::GameCamera::setFovy(float fovy) { mCameraData.fovy = fovy; }
=======
inline void Techstorm::GameCamera::setFovy(float fovy) { mCameraData.fovy = fovy; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
void Atlas::GameCamera::setFovy(float fovy) { mCameraData.fovy = fovy; }
>>>>>>> main

/// <summary>
/// Sets the projection.
/// </summary>
/// <param name="projection">The projection.</param>
<<<<<<< HEAD
<<<<<<< HEAD
void Techstorm::GameCamera::setProjection(int projection) { mCameraData.projection = projection; }
=======
inline void Techstorm::GameCamera::setProjection(int projection) { mCameraData.projection = projection; }
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
=======
void Atlas::GameCamera::setProjection(int projection) { mCameraData.projection = projection; }
>>>>>>> main
