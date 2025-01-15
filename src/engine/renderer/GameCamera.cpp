#include "GameCamera.h"
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

  Vector3 Atlas::GameCamera::getPosition() const { return mCameraData.position; }

/// <summary>
/// Gets the target.
/// </summary>
/// <returns></returns>

  Vector3 Atlas::GameCamera::getTarget() const { return mCameraData.target; }

/// <summary>
/// Gets up.
/// </summary>
/// <returns></returns>
  Vector3 Atlas::GameCamera::getUp() const { return mCameraData.up; }

/// <summary>
/// Gets the camera's fovy (Field of View Y). For more information, please see the section on it in the documentation.
/// </summary>
/// <returns>The camera's fovy</returns>
/// \ref how-cameras-work
  float Atlas::GameCamera::getFovy() const { return mCameraData.fovy; }

/// <summary>
/// Gets the camera's projection.
/// </summary>
/// <returns></returns>
  int Atlas::GameCamera::getProjection() const { return mCameraData.projection; }

/// <summary>
/// Sets the camera data.
/// </summary>
/// <param name="data">The data.</param>
  void Atlas::GameCamera::setCameraData(Camera3D const& data) { mCameraData = data; }

/// <summary>
/// Sets the position.
/// </summary>
/// <param name="position">The position.</param>
void Atlas::GameCamera::setPosition(Vector3 position) { mCameraData.position = position; }

/// <summary>
/// Sets the target.
/// </summary>
/// <param name="target">The target.</param>
void Atlas::GameCamera::setTarget(Vector3 target) { mCameraData.target = target; }

/// <summary>
/// Sets up.
/// </summary>
/// <param name="up">Up.</param>
void Atlas::GameCamera::setUp(Vector3 up) { mCameraData.up = up; }

/// <summary>
/// Sets the fovy.
/// </summary>
/// <param name="fovy">The fovy.</param>
void Atlas::GameCamera::setFovy(float fovy) { mCameraData.fovy = fovy; }

/// <summary>
/// Sets the projection.
/// </summary>
/// <param name="projection">The projection.</param>
void Atlas::GameCamera::setProjection(int projection) { mCameraData.projection = projection; }
