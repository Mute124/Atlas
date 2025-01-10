#pragma once
#include <vector>
#include <memory>

#include "../utils/Singleton.h"

#include "GameObjectRegistry.h"
#include "GameCamera.h"

#include <raylib.h>

namespace Atlas {
	class GameObject;

	/// <summary>
	/// Serves as the renderer for the game.
	/// </summary>
	class Renderer {
	public:
#ifndef TS_CUSTOM_RENDERER_CONSTRUCTOR
		/// <summary>
		/// Initializes a new instance of the <see cref="Renderer"/> class.
		/// </summary>
		Renderer() {
			//initialize();
			mCamera = GameCamera();
			mCamera.setPosition(Vector3{ 0.0f, 10.0f, 4.0f });
			mCamera.setTarget(Vector3{ 0.0f, 2.0f, 0.0f });
			mCamera.setUp(Vector3{ 0.0f, 1.0f, 0.0f });
			mCamera.setFovy(60.0f);
			mCamera.setProjection(CAMERA_PERSPECTIVE);
		}
#endif
		/// <summary>
		/// Initializes this instance.
		/// </summary>
		void initialize();

		/// <summary>
		/// Renders the render texture to the screen using the specified camera.
		/// </summary>
		/// <param name="cam">The cam.</param>
		void render(GameCamera& cam);

		/// <summary>
		/// Textures the scene to the render texture using the specified camera.
		/// </summary>
		/// <param name="cam">The cam.</param>
		void texture(GameCamera& cam);

		void update();

		/// <summary>
		/// Draws the FBO/RenderTexture2D.
		/// </summary>
		void drawFBO();

		void addStandaloneDrawCall(std::function<void()> drawCall);


		/// <summary>
		/// Adds the game object.
		/// </summary>
		/// <param name="gameObject">The game object pointer to add.</param>
		void addGameObject(IGameObject* gameObject);

		/// <summary>
		/// Removes a game object.
		/// </summary>
		/// <param name="gameObject">The game object pointer to remove.</param>
		void removeGameObject(IGameObject* gameObject);

		/// <summary>
		/// Adds a game object gate.
		/// </summary>
		/// <param name="gameObjectGate">The game object gate pointer to add.</param>
		/// <seealso cref="IGameObjectGate" />
		void addGameObjectGate(IGameObjectGate* gameObjectGate);

		/// <summary>
		/// Removes a game object gate.
		/// </summary>
		/// <param name="gameObjectGate">The game object gate pointer to remove.</param>
		/// <seealso cref="IGameObjectGate" />
		void removeGameObjectGate(IGameObjectGate* gameObjectGate);

		/// <summary>
		/// Cleanups this instance.
		/// </summary>
		void cleanup();

		GameCamera mCamera;
	private:
		GameObjectRegistry mGameObjects;
		RenderTexture2D mScreenBuffer;
		Color mBackgroundColor = BLACK;
		Color mFBOTint = WHITE;
		
		
#ifdef TS_RENDERER_2D
		/// <summary>
		/// Renders the 2d scene.
		/// </summary>
		/// <param name="cam">The active camera.</param>
		void render2D(GameCamera& cam);
#else
		/// <summary>
		/// Renders the 3d scene.
		/// </summary>
		/// <param name="cam">The active camera.</param>
		void render3D(GameCamera& cam);
#endif
	};

}
