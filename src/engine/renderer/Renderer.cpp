#include "Renderer.h"

#include "backend/RenderingBackend.h"
#include "window/Window.h"
#include "../core/Core.h"
#include "backend/VulkanRenderingBackend.h"

//
//#include <iostream>
///*
//// unused, but here for a bit
//Camera ConvertCamera(Atlas::CameraData* data) {
//	using namespace Atlas;
//	Camera cam = Camera();
//
//	// convert vectors in data to raylib vectors
//	Vector3 position = { data->position.x, data->position.y, data->position.z };
//	Vector3 target = { data->target.x, data->target.y, data->target.z };
//	Vector3 up = { data->up.x, data->up.y, data->up.z };
//
//	cam.position = position;
//	cam.target = target;
//	cam.up = up;
//	cam.fovy = data->fovy;
//	cam.projection = data->projection;
//
//	return cam;
//}
//*/
//
//
///// <summary>
///// Initializes a new instance of the <see cref="Renderer"/> class.
///// </summary>
//Atlas::Renderer::Renderer() {
//	//initialize();
//	mCamera = GameCamera();
//	mCamera.setPosition(Vector3{0.0f, 10.0f, 4.0f});
//	mCamera.setTarget(Vector3{0.0f, 2.0f, 0.0f});
//	mCamera.setUp(Vector3{0.0f, 1.0f, 0.0f});
//	mCamera.setFovy(60.0f);
//	mCamera.setProjection(CAMERA_PERSPECTIVE);
//
//}
//
//void Atlas::Renderer::init()
//{
//	std::cout << "Initializing renderer" << std::endl;
//
//	
//	// TODO: Implement Renderer::initialize
//	Vector2 size = { GetScreenWidth(), GetScreenHeight() };
//	this->mScreenBuffer = LoadRenderTexture(size.x, size.y);
//	mCamera.init();
//}
//
//void Atlas::Renderer::render(GameCamera& cam)
//{
//#ifdef ATLAS_RENDERER_2D
//	render2D(cam);
//#else
//	render3D(cam);
//#endif
//}
//
//void Atlas::Renderer::texture(GameCamera& cam)
//{
//	std::scoped_lock<std::mutex> lock(this->mMutex);
//	BeginTextureMode(this->mScreenBuffer);
//	ClearBackground(this->mBackgroundColor);
//#ifdef ATLAS_RENDERER_2D
//#else
//	BeginMode3D(cam.getCameraData());
//#endif
//
//	mGameObjects.texture();
//	
//#ifdef ATLAS_RENDERER_2D
//#else
//
//	//DrawCube(Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, RED);
//
//	EndMode3D();
//#endif
//	EndTextureMode();
//}
//
//void Atlas::Renderer::update()
//{
//	updateCamera();
//}
//
//void Atlas::Renderer::drawFBO() {
//	// TODO: Allow FBO shaders
//
//	DrawTextureRec(this->mScreenBuffer.texture, Rectangle{ 0, 0, (float)(this->mScreenBuffer.texture.width), -(float)(this->mScreenBuffer.texture.height) }, Vector2{ 0, 0 }, this->mFBOTint);
//}
//
// void Atlas::Renderer::addStandaloneDrawCall(std::function<void()> drawCall) { 
//	 mGameObjects.addStandaloneDrawCall(drawCall); 
//	 //this->mDrawCalls.push_back(drawCall); 
//}
//
//void Atlas::Renderer::addGameObject(IGameObject* gameObject)
//{
//
//	this->mGameObjects.addGameObject(gameObject);
//}
//
//void Atlas::Renderer::removeGameObject(IGameObject* gameObject)
//{
//	//this->mGameObjects.erase(std::remove(this->mGameObjects.begin(), this->mGameObjects.end(), gameObject), this->mGameObjects.end());
//}
//
//void Atlas::Renderer::addGameObjectGate(IGameObjectGate* gameObjectGate) {}
//
//void Atlas::Renderer::removeGameObjectGate(IGameObjectGate* gameObjectGate) {}
//
//void Atlas::Renderer::updateObjects()
//{
//	try {
//		std::scoped_lock<std::mutex> lock(this->mMutex);
//
//		mGameObjects.update();
//	} catch (const std::exception& e) {
//		std::cout << e.what() << std::endl;
//	}
//
//}
//
//void Atlas::Renderer::updateCamera()
//{
//	mCamera.update();
//}
//
//void Atlas::Renderer::cleanup()
//{
//}
//
//#ifdef ATLAS_RENDERER_2D
//void Atlas::Renderer::render2D(GameCamera& cam)
//{
//}
//#else
//void Atlas::Renderer::render3D(GameCamera& cam)
//{
//
//	//UpdateCamera(&cam.mCameraData, CAMERA_FIRST_PERSON);
//
//	//texture(cam);
//
//	BeginDrawing();
//	drawFBO();
//
//	mGameObjects.render();
//	EndDrawing();
//}
//
//#endif

Atlas::IRenderer::IRenderer(IGameWindow* gameWindow, IRenderingBackend* backend, bool canBeMultiThreaded)
	: mainGameWindow(gameWindow), renderingBackend(backend), mCanBeMultiThreaded(canBeMultiThreaded)
{
}

Atlas::IRenderer::IRenderer(IGameWindow* gameWindow, IRenderingBackend* backend)
	: mainGameWindow(gameWindow), renderingBackend(backend)
{
}

bool Atlas::IRenderer::canBeMultiThreaded() const { return this->mCanBeMultiThreaded; }

bool Atlas::IRenderer::isInitialized() const { return this->mIsInitialized; }

#ifdef ATLAS_USE_VULKAN

Atlas::VulkanRenderer::VulkanRenderer(IGameWindow* gameWindow, VulkanRenderingBackend* backend)
	: IRenderer(gameWindow, backend, true)
{
}

Atlas::VulkanRenderer::VulkanRenderer(IGameWindow* gameWindow)
	: VulkanRenderer(gameWindow, new VulkanRenderingBackend())
{
}

void Atlas::VulkanRenderer::init()
{
	this->mainGameWindow->init();
	this->renderingBackend->init();
}

void Atlas::VulkanRenderer::update()
{
	while (!this->mainGameWindow->shouldClose()) {
		if (this->mainGameWindow != nullptr) {
			this->mainGameWindow->update();
		}
		else {
#ifdef ATLAS_DEBUG

			std::cout << "Main game window is null!" << std::endl;
#endif
		}

		if (this->renderingBackend != nullptr) {

			this->renderingBackend->update();
		}
		else {
#ifdef ATLAS_DEBUG
			std::cout << "Rendering backend is null!" << std::endl;
#endif
		}
	}
}

void Atlas::VulkanRenderer::cleanup()
{
	this->mainGameWindow->close(true);
}

#endif