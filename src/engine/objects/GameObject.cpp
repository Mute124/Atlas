#include "GameObject.h"
#include "../components/TransformComponent.h"
#include "../components/CleanupEventArgs.h"
#include "../components/DestroyEventArgs.h"
#include "../components/PostUpdateEventArgs.h"
#include "../components/PreUpdateEventArgs.h"
#include "../components/RenderEventArgs.h"
#include "../components/TextureEventArgs.h"
#include "../components/UpdateEventArgs.h"
#include "IGameObject.h"
#include <stdexcept>

Atlas::GameObject::GameObject() : IGameObject() {
	//addComponent<TransformComponent>();
}

void Atlas::GameObject::preUpdate() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->preUpdate(new PreUpdateEventArgs());
		}
	}
}

void Atlas::GameObject::update() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->update(new UpdateEventArgs());
		}
		else {
			throw std::invalid_argument("Component " + key + "cannot be null");
		}
	}
}

void Atlas::GameObject::postUpdate() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->postUpdate(new PostUpdateEventArgs());
		}
		else {
			throw std::invalid_argument("Component " + key + "cannot be null");
		}
	}
}

void Atlas::GameObject::texture() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->texture(new TextureEventArgs());
		}
		else {
			throw std::invalid_argument("Component " + key + "cannot be null");
		}
	}
}

void Atlas::GameObject::render() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->render(new RenderEventArgs());
		}
		else {
			throw std::invalid_argument("Component " + key + "cannot be null");
		}
	}
}

void Atlas::GameObject::destroy() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->destroy(new DestroyEventArgs());
		}
		else {
			throw std::invalid_argument("Component " + key + "cannot be null");
		}
	}
}

void Atlas::GameObject::cleanup() {
	for (auto const& [key, component] : mComponents) {
		if (component.get() != nullptr) {
			component->cleanup(new CleanupEventArgs());
		}
		else {
			throw std::invalid_argument("Component " + key + "cannot be null");
		}
	}
}
