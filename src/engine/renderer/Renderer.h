#pragma once
#include <vector>
#include <memory>

#include "../utils/Singleton.h"

#include "../objects/GameObjectRegistry.h"
#include "GameCamera.h"
<<<<<<< HEAD

<<<<<<< HEAD
=======
>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc


=======
>>>>>>> main
#include <raylib.h>

namespace Atlas {
	class GameObject;

	/// <summary>
	/// Serves as the renderer for the game.
	/// </summary>
	class Renderer {
	public:
#ifndef ATLAS_CUSTOM_RENDERER_CONSTRUCTOR
		/// <summary>
		/// Initializes a new instance of the <see cref="Renderer"/> class.
		/// </summary>
		Renderer();
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
<<<<<<< HEAD
		
<<<<<<< HEAD
		
=======
		GameCamera mCamera;


>>>>>>> 54653e5aab996b3ca5dfae6c481ea281d8cba5dc
#ifdef TS_RENDERER_2D
=======
#ifdef ATLAS_RENDERER_2D
>>>>>>> main
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
