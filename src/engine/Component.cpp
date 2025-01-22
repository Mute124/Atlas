#include "Component.h"

Atlas::Component::Component(const std::shared_ptr<IGameObject>& mOwner)
	: mOwner(mOwner)
{
}

Atlas::Component::~Component() {

}

void Atlas::Component::preUpdate() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::update() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::postUpdate() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::render() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::texture() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::destroy() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::cleanup() {
	// Empty because this is a base class and the user is supposed to override these if they need.
}

std::shared_ptr<Atlas::IGameObject> Atlas::Component::getOwner() {
	if (mOwner == nullptr) {
		return nullptr;
	}

	return mOwner;
}
