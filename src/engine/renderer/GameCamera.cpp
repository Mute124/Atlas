#include "GameCamera.h"
#include "../Project.h"
#include <raylib.h>
#include "../input/Input.h"
#include <string>

Atlas::GameCamera::GameCamera() {}

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

	// setup input
	InputRegistry* inputRegistry = GetAtlasEngine()->getInputRegistry().get();

	// forwards
	inputRegistry->registerActionCallback(
		new CameraInputAction("Camera Move Forward", KEY_W, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_DOWN, this),
		[](InputAction* action) {
			CameraInputAction* cameraAction = static_cast<CameraInputAction*>(action);
			cameraAction->moveForward(1.0f);
		}
	);

	// backwards
	inputRegistry->registerActionCallback(
		new CameraInputAction("Camera Move Backward", KEY_S, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_DOWN, this),
		[](InputAction* action) {
			CameraInputAction* cameraAction = static_cast<CameraInputAction*>(action);
			cameraAction->moveBackward(1.0f);
		}
	);

	// left
	inputRegistry->registerActionCallback(
		new CameraInputAction("Camera Move Left", KEY_A, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_DOWN, this),
		[](InputAction* action) {
			CameraInputAction* cameraAction = static_cast<CameraInputAction*>(action);
			cameraAction->moveLeft(1.0f);
		}
	);

	// right
	inputRegistry->registerActionCallback(
		new CameraInputAction("Camera Move Right", KEY_D, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_DOWN, this),
		[](InputAction* action) {
			CameraInputAction* cameraAction = static_cast<CameraInputAction*>(action);
			cameraAction->moveRight(1.0f);
		}
	);

	// up
	inputRegistry->registerActionCallback(
		new CameraInputAction("Camera Move Up", KEY_SPACE, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_DOWN, this),
		[](InputAction* action) {
			CameraInputAction* cameraAction = static_cast<CameraInputAction*>(action);
			cameraAction->moveUp(1.0f);
		}
	);

	// down
	inputRegistry->registerActionCallback(
		new CameraInputAction("Camera Move Down", KEY_LEFT_CONTROL, EInputType::INPUT_KEYBOARD, EInputTrigger::INPUT_TRIGGER_DOWN, this),
		[](InputAction* action) {
			CameraInputAction* cameraAction = static_cast<CameraInputAction*>(action);
			cameraAction->moveDown(1.0f);
		}
	);
}

void Atlas::GameCamera::update()
{
	//UpdateCamera(&mCameraData, CAMERA_FIRST_PERSON);
	UpdateCameraPro(&mCameraData,
		Vector3 {
			// Move forward-backward
			this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::FORWARD_DIRECTION)] -
			this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::BACKWARD_DIRECTION)],

			// Move right-left
			this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::RIGHT_DIRECTION)] -
			this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::LEFT_DIRECTION)],

			// Move up-down
			this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::UP_DIRECTION)] -
			this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::DOWN_DIRECTION)]
		},
		Vector3 {
			GetMouseDelta().x * 0.05f,                            // Rotation: yaw
			GetMouseDelta().y * 0.05f,                            // Rotation: pitch
			0.0f                                                // Rotation: roll
		},
		GetMouseWheelMove() * 2.0f
	);                              // Move to target (zoom)

	// clear directional input
	this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::FORWARD_DIRECTION)] = 0.0f;
	this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::BACKWARD_DIRECTION)] = 0.0f;
	this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::LEFT_DIRECTION)] = 0.0f;
	this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::RIGHT_DIRECTION)] = 0.0f;
	this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::UP_DIRECTION)] = 0.0f;
	this->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::DOWN_DIRECTION)] = 0.0f;

	/*	this->mDirectionalInput[GameCamera::EMovementDirection::FORWARD_DIRECTION].reset();
	this->mDirectionalInput[GameCamera::EMovementDirection::BACKWARD_DIRECTION].reset();
	this->mDirectionalInput[GameCamera::EMovementDirection::LEFT_DIRECTION].reset();
	this->mDirectionalInput[GameCamera::EMovementDirection::RIGHT_DIRECTION].reset();
	this->mDirectionalInput[GameCamera::EMovementDirection::UP_DIRECTION].reset();
	this->mDirectionalInput[GameCamera::EMovementDirection::DOWN_DIRECTION].reset();*/
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

Atlas::GameCamera* Atlas::GameCamera::GetMainCamera() { return GameCamera::sMainCamera; }

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

void Atlas::GameCamera::setAsMainCamera() {
	sMainCamera = this;
	mIsMainCamera = true;
}


bool Atlas::GameCamera::isMainCamera() const { return mIsMainCamera; }

Atlas::GameCamera::CameraInputAction::CameraInputAction(const std::string& name, int key, const EInputType& type, const EInputTrigger& trigger,
	GameCamera* camera) : InputAction(name, "renderer", key, type, trigger), camera(camera) {
	
	}

void Atlas::GameCamera::CameraInputAction::moveForward(float amount)
{
	camera->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::FORWARD_DIRECTION)] += amount;
}

void Atlas::GameCamera::CameraInputAction::moveBackward(float amount)
{
	camera->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::BACKWARD_DIRECTION)] += amount;
}

void Atlas::GameCamera::CameraInputAction::moveLeft(float amount)
{
	camera->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::LEFT_DIRECTION)] += amount;
}

void Atlas::GameCamera::CameraInputAction::moveRight(float amount)
{
	camera->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::RIGHT_DIRECTION)] += amount;
}

void Atlas::GameCamera::CameraInputAction::moveUp(float amount)
{
	camera->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::UP_DIRECTION)] += amount;
}

void Atlas::GameCamera::CameraInputAction::moveDown(float amount)
{
	camera->mDirectionalInput[static_cast<size_t>(GameCamera::EMovementDirection::DOWN_DIRECTION)] += amount;
}
