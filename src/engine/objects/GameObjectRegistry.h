#pragma once
#include <iostream>
#include <vector>
#include "IGameObject.h"
#include "../Common.h"
#include "IGameObjectGate.h"
#include <functional>
#include <map>
#include <string>

namespace Atlas {
	/// <summary>
	/// Game object registry, it stores all game objects in the world and manages their updating, rendering, destruction, creation, etc.
	/// </summary>
	class GameObjectRegistry {
	private:
		std::vector<std::function<void()>> mStandaloneDrawCalls;
		std::vector<IGameObject*> mGameObjectsSink;
		std::vector<IGameObjectGate*> mGameObjectsGateSink;

		//std::map<std::string, std::vector<IGameObjectGate*>> mGameObjectGates;

		friend class Renderer;
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

		void addStandaloneDrawCall(std::function<void()> drawCall) { this->mStandaloneDrawCalls.push_back(drawCall); }

		/// <summary>
		/// Updates all game objects in the instance (world).
		/// </summary>
		void update();

		/// <summary>
		/// Renders all game objects in the instance (world).
		/// </summary>
		void render();

		void texture() {
			for (auto& obj : this->mGameObjectsSink) {
				obj->texture();
			}
		}


	};
}
