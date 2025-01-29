#pragma once
#include "Component.h"
namespace Atlas {
	class ObjectRendererComponent abstract : public Component {
	public:

		explicit ObjectRendererComponent(const std::shared_ptr<IGameObject>& mOwner) : Component(mOwner) {}
		ObjectRendererComponent() : Component() {}
	};
}