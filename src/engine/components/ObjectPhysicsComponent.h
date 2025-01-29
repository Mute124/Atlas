#pragma once
#include "Component.h"
#include <memory>

#include "../physics/Physics.h"

namespace Atlas {
	class ObjectPhysicsComponent : public Component {
	private:
		JPH::BodyID setupPhysics() {
			// TODO: this
		}
	public:

		JPH::BodyID mBodyID;
		JPH::BodyInterface& mBodyInterface = PhysicsEngine::Instance().getBodyInterface();

		ObjectPhysicsComponent() : Component() {
			
		}

		explicit ObjectPhysicsComponent(const std::shared_ptr<IGameObject>& mOwner) : Component(mOwner) {
		}
	};
}