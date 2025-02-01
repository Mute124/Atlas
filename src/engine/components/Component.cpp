#include "Component.h"
#include "../objects/IGameObject.h"

Atlas::Component::Component(IGameObject* mOwner)
	: mOwner(mOwner)
{
}

Atlas::Component::~Component() {

}

void Atlas::Component::preUpdate(PreUpdateEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::update(UpdateEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::postUpdate(PostUpdateEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::render(RenderEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::texture(TextureEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::destroy(DestroyEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

void Atlas::Component::cleanup(CleanupEventArgs* args)
{
	// Empty because this is a base class and the user is supposed to override these if they need.
}

std::shared_ptr<Atlas::ObjectHolder> Atlas::Component::getOwner() {
	if (mOwner == nullptr) {
		return nullptr;
	}

	
	return std::make_shared<Atlas::ObjectHolder>(mOwner);
}
