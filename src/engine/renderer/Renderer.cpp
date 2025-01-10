#include "Renderer.h"

#include <iostream>
/*
// unused, but here for a bit
Camera ConvertCamera(Atlas::CameraData* data) {
	using namespace Atlas;
	Camera cam = Camera();

	// convert vectors in data to raylib vectors
	Vector3 position = { data->position.x, data->position.y, data->position.z };
	Vector3 target = { data->target.x, data->target.y, data->target.z };
	Vector3 up = { data->up.x, data->up.y, data->up.z };

	cam.position = position;
	cam.target = target;
	cam.up = up;
	cam.fovy = data->fovy;
	cam.projection = data->projection;

	return cam;
}
*/

void Atlas::Renderer::initialize()
{
	std::cout << "Initializing renderer" << std::endl;
	// TODO: Implement Renderer::initialize
	Vector2 size = { GetScreenWidth(), GetScreenHeight() };
	this->mScreenBuffer = LoadRenderTexture(size.x, size.y);
}

void Atlas::Renderer::render(GameCamera& cam)
{
#ifdef TS_RENDERER_2D
	render2D(cam);
#else
	render3D(cam);
#endif
}

void Atlas::Renderer::texture(GameCamera& cam)
{
	BeginTextureMode(this->mScreenBuffer);
	ClearBackground(this->mBackgroundColor);
#ifdef TS_RENDERER_2D
#else
	BeginMode3D(cam.getCameraData());
#endif

	mGameObjects.texture();
	DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, RED);
#ifdef TS_RENDERER_2D
#else
	EndMode3D();
#endif
	EndTextureMode();
}

void Atlas::Renderer::update()
{
	mCamera.update();
}

void Atlas::Renderer::drawFBO() {
	// TODO: Allow FBO shaders

	DrawTextureRec(this->mScreenBuffer.texture, Rectangle{ 0, 0, (float)(this->mScreenBuffer.texture.width), -(float)(this->mScreenBuffer.texture.height) }, Vector2{ 0, 0 }, this->mFBOTint);
}

 void Atlas::Renderer::addStandaloneDrawCall(std::function<void()> drawCall) { 
	 mGameObjects.addStandaloneDrawCall(drawCall); 
	 //this->mDrawCalls.push_back(drawCall); 
}

void Atlas::Renderer::addGameObject(IGameObject* gameObject)
{
	this->mGameObjects.addGameObject(gameObject);
}

void Atlas::Renderer::removeGameObject(IGameObject* gameObject)
{
	//this->mGameObjects.erase(std::remove(this->mGameObjects.begin(), this->mGameObjects.end(), gameObject), this->mGameObjects.end());
}

void Atlas::Renderer::addGameObjectGate(IGameObjectGate* gameObjectGate) {}

void Atlas::Renderer::removeGameObjectGate(IGameObjectGate* gameObjectGate) {}

void Atlas::Renderer::cleanup()
{
}

#ifdef TS_RENDERER_2D
void Atlas::Renderer::render2D(GameCamera& cam)
{
}
#else
void Atlas::Renderer::render3D(GameCamera& cam)
{
	//UpdateCamera(&cam.mCameraData, CAMERA_FIRST_PERSON);

	//texture(cam);

	BeginDrawing();
	drawFBO();

	mGameObjects.render();
	EndDrawing();
}

#endif
