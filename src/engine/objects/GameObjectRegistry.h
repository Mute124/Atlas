#pragma once
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "../core/Common.h"
#include "../renderer/ICuller.h"
#include "IGameObject.h"
#include "IGameObjectGate.h"

namespace Atlas {
	/**
	 * @brief Stores and manages the states of all IGameObject (and the derivative classes thereof) in the world
	 * @since v0.0.1
	 */
	class GameObjectRegistry {
	private:
		std::vector<std::function<void()>> mStandaloneDrawCalls;
		std::vector<IGameObject*> mGameObjectsSink;
		std::vector<IGameObjectGate*> mGameObjectsGateSink;
		ICuller* mCuller = nullptr;

		//std::map<std::string, std::vector<IGameObjectGate*>> mGameObjectGates;

		friend class Renderer;

		std::vector<IGameObject*> copyGameObjectSink();
		
	public:

		/// <summary>
		/// Adds a game object pointer to mGameObjectsSink.
		/// </summary>
		/// <param name="gameObject">The game object pointer to add.</param>
		/// <seealso cref="GameObject"/>
		void addGameObject(IGameObject* gameObject);

		/// <summary>
		/// Adds a game object gate pointer of IGameObjectGate to mGameObjectsGateSink.
		/// </summary>
		/// <param name="gameObjectGate">The game object gate.</param>
		/// <seealso cref="IGameObjectGate"/>
		void addGameObjectGate(IGameObjectGate* gameObjectGate);

		void addStandaloneDrawCall(std::function<void()> drawCall);

		/// <summary>
		/// Updates all game objects in the instance (world).
		/// </summary>
		void update();

		/// <summary>
		/// Renders all game objects in the instance (world).
		/// </summary>
		void render();

		void texture();


	};
}
