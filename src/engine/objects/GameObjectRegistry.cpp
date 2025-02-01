#include "GameObjectRegistry.h"

void Atlas::GameObjectRegistry::addGameObject(IGameObject* gameObject)
{
	std::cout << "Pushing GameObject of type " << typeid(*gameObject).name() << " to GameObjectRegistry" << std::endl;
	if (gameObject != nullptr) {
		for (auto& gate : mGameObjectsGateSink) {
			gate->process(gameObject);
		}

		this->mGameObjectsSink.push_back(gameObject);
		std::cout << "GameObjectRegistry::addGameObject: GameObject added" << std::endl;
	}
	else {
		std::cout << "GameObjectRegistry::addGameObject: GameObject is null" << std::endl;
	}
}
void Atlas::GameObjectRegistry::addGameObjectGate(IGameObjectGate* gameObjectGate) {
	if (gameObjectGate != nullptr) {
		this->mGameObjectsGateSink.push_back(gameObjectGate);
	}
}

void Atlas::GameObjectRegistry::update()
{
	std::vector<IGameObject*> gameObjects = this->mGameObjectsSink; // copy for thread safety
	for (auto& obj : gameObjects) {
		obj->update();
	}
}

void Atlas::GameObjectRegistry::render()
{
	std::vector<IGameObject*> gameObjects = this->mGameObjectsSink; // copy for thread safety

	for (auto& drawCall : this->mStandaloneDrawCalls) {
		drawCall();
	}

	for (auto& obj : gameObjects) {

		obj->render();
	}
}
