#pragma once
#include "IGameObject.h"
#include "../physics/Physics.h"
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/EActivation.h>
namespace Atlas {
	class IPhysicsGameObject abstract : public IGameObject {
	public:
		JPH::BodyID id;
		JPH::BodyInterface& bodyInterface;

		IPhysicsGameObject(JPH::BodyCreationSettings const& bodySettings, JPH::EActivation activationState) : IGameObject(), bodyInterface(PhysicsEngine::Instance().getBodyInterface()) {
			this->id = bodyInterface.CreateAndAddBody(bodySettings, activationState);
		}

		~IPhysicsGameObject() override {
			this->bodyInterface.RemoveBody(this->id);
		}

		/// <summary>
		/// Action on collision validation.
		/// </summary>
		/// <param name="other">The other game object.</param>
		virtual void onCollisionValidation(IGameObject* other) = 0;

		/// <summary>
		/// Action upon entering collision. This is called the frame the collision happens.
		/// </summary>
		/// <param name="other">The other game object.</param>
		virtual void onCollisionEnter(IGameObject* other) = 0;

		/// <summary>
		/// Action upon exiting collision.
		/// </summary>
		/// <param name="other">The other game object.</param>
		virtual void onCollisionExit(IGameObject* other) = 0;

		/// <summary>
		/// Action upon remaining in collision.
		/// </summary>
		/// <param name="other">The other game object.</param>
		/// <inheritdoc />
		virtual void onCollisionStay(IGameObject* other) = 0;
	};
}