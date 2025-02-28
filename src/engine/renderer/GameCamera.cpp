#include "GameCamera.h"
#include "../Project.h"
#include <raylib.h>
#include "../input/Input.h"
#include <string>

size_t Atlas::GameCamera::GetDirectionalInputAsIndex(const EMovementDirection& direction) {
	return static_cast<size_t>(direction);
}

Atlas::GameCamera::GameCamera() {}

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
	// This is being set here so that the function can follow the principles of DRY -- Don't Repeat Yourself
	size_t convertedForwards = static_cast<size_t>(GameCamera::EMovementDirection::FORWARD_DIRECTION);
	size_t convertedBackwards = static_cast<size_t>(GameCamera::EMovementDirection::BACKWARD_DIRECTION);
	size_t convertedLeft = static_cast<size_t>(GameCamera::EMovementDirection::LEFT_DIRECTION);
	size_t convertedRight = static_cast<size_t>(GameCamera::EMovementDirection::RIGHT_DIRECTION);
	size_t convertedUp = static_cast<size_t>(GameCamera::EMovementDirection::UP_DIRECTION);
	size_t convertedDown = static_cast<size_t>(GameCamera::EMovementDirection::DOWN_DIRECTION);

	//UpdateCamera(&mCameraData, CAMERA_FIRST_PERSON);
	UpdateCameraPro(&mCameraData,
		Vector3 {
			// Move forward-backward
			this->mDirectionalInput[convertedForwards] -
			this->mDirectionalInput[convertedBackwards],

			// Move right-left
			this->mDirectionalInput[convertedRight] -
			this->mDirectionalInput[convertedLeft],

			// Move up-down
			this->mDirectionalInput[convertedUp] -
			this->mDirectionalInput[convertedDown]
		},
		Vector3 {
			GetMouseDelta().x * 0.05f,                            // Rotation: yaw
			GetMouseDelta().y * 0.05f,                            // Rotation: pitch
			0.0f                                                  // Rotation: roll
		},
		GetMouseWheelMove() * 2.0f                                // Move to target (zoom)
	);          
	/****************************************************
	* Clear directional input
	*****************************************************/

	this->mDirectionalInput[convertedForwards] = 0.0f;
	this->mDirectionalInput[convertedBackwards] = 0.0f;

	this->mDirectionalInput[convertedLeft] = 0.0f;
	this->mDirectionalInput[convertedRight] = 0.0f;

	this->mDirectionalInput[convertedUp] = 0.0f;
	this->mDirectionalInput[convertedDown] = 0.0f;

}

Camera3D Atlas::GameCamera::getCameraData() { 
	return mCameraData; 
}

Vector3 Atlas::GameCamera::getPosition() const {
	  return mCameraData.position;
}

Vector3 Atlas::GameCamera::getTarget() const {
	return mCameraData.target; 
}


Vector3 Atlas::GameCamera::getUp() const { 
	return mCameraData.up; 
}


float Atlas::GameCamera::getFovy() const { 
	return mCameraData.fovy;
}

int Atlas::GameCamera::getProjection() const { 
	return mCameraData.projection; 
}

Atlas::GameCamera* Atlas::GameCamera::GetMainCamera() { 
	return GameCamera::sMainCamera;
}

void Atlas::GameCamera::setCameraData(Camera3D const& data) { 
	mCameraData = data; 
}

void Atlas::GameCamera::setPosition(Vector3 position) {
	mCameraData.position = position;
}


void Atlas::GameCamera::setTarget(Vector3 target) { 
	mCameraData.target = target; 
}


void Atlas::GameCamera::setUp(Vector3 up) { 
	mCameraData.up = up; 
}


void Atlas::GameCamera::setFovy(float fovy) { 
	mCameraData.fovy = fovy; 
}

void Atlas::GameCamera::setProjection(int projection) {
	mCameraData.projection = projection; 
}

void Atlas::GameCamera::setAsMainCamera() {
	sMainCamera = this;
	mIsMainCamera = true;
}


bool Atlas::GameCamera::isMainCamera() const { 
	return mIsMainCamera; 
}

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
