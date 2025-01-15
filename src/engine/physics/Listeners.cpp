#include "Listeners.h"

// See: ContactListener

  JPH::ValidateResult Atlas::ObjectContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
	//cout << "Contact validate callback" << endl;

	// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

  void Atlas::ObjectContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	//cout << "A contact was added" << endl;
}

  void Atlas::ObjectContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	//cout << "A contact was persisted" << endl;
}

  void Atlas::ObjectContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
	//cout << "A contact was removed" << endl;
}

  void Atlas::MyBodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{
	//cout << "A body got activated" << endl;
}

  void Atlas::MyBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData)
{
	//cout << "A body went to sleep" << endl;
}

  void Atlas::ContactListenerGate::process(ContactListenerGateParams entree)
  {
  }
