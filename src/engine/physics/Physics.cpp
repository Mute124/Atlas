#include "Physics.h"
#include <thread>
#include <Jolt/Core/Core.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>

  Techstorm::AllocatedPhysicsResources::AllocatedPhysicsResources() {}

  Techstorm::AllocatedPhysicsResources::AllocatedPhysicsResources(AllocatedPhysicsResources const& resources) : cMaxBodies(resources.cMaxBodies), cNumBodyMutexes(resources.cNumBodyMutexes), cMaxBodyPairs(resources.cMaxBodyPairs), cMaxContactConstraints(resources.cMaxContactConstraints) {}

  Techstorm::AllocatedPhysicsResources::AllocatedPhysicsResources(const JPH::uint& cMaxBodies, const JPH::uint& cNumBodyMutexes, const JPH::uint& cMaxBodyPairs, const JPH::uint& cMaxContactConstraints)
	: cMaxBodies(cMaxBodies), cNumBodyMutexes(cNumBodyMutexes), cMaxBodyPairs(cMaxBodyPairs), cMaxContactConstraints(cMaxContactConstraints)
{
}

  void Techstorm::PhysicsEngine::init(const AllocatedPhysicsResources resources) {
	if (!mIsInitialized) {
		mResources = resources;
		JPH::RegisterDefaultAllocator();

		// Install trace and assert callbacks
		JPH::Trace = TraceImpl;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

			JPH::Factory::sInstance = new JPH::Factory();

		JPH::RegisterTypes();

		mTempAllocator = new JPH::TempAllocatorImpl(mResources.preAllocatedMemory);

		mJobSystemThreadPool = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);


		mPhysicsSystem = new JPH::PhysicsSystem();
		// Now we can create the actual physics system. This is not nullptr.
		mPhysicsSystem->Init(
			resources.cMaxBodies,
			resources.cNumBodyMutexes,
			resources.cMaxBodyPairs,
			resources.cMaxContactConstraints,
			mBroadPhaseLayerInterface,
			mObjectVsBroadPhaseLayerFilter,
			mObjectLayerPairFilter
		);

		mPhysicsSystem->SetBodyActivationListener(mBodyActivationListener);
		mPhysicsSystem->SetContactListener(mContactListener);

		mBodyInterfaceHolder = new BodyInterfaceHolder(mPhysicsSystem->GetBodyInterface());

	}
}

  void Techstorm::PhysicsEngine::update(const float cDeltaTime) {
	mPhysicsSystem->Update(cDeltaTime, mCollisionSteps, 4, mTempAllocator, mJobSystemThreadPool);
}

  void Techstorm::PhysicsEngine::optimizeBroadPhase() {
	mPhysicsSystem->OptimizeBroadPhase();
}

  JPH::BodyInterface& Techstorm::PhysicsEngine::getBodyInterface() const { return mBodyInterfaceHolder->bodyInterface; }

  Techstorm::PhysicsEngine::BodyInterfaceHolder::BodyInterfaceHolder(JPH::BodyInterface& bodyInterface) : bodyInterface(bodyInterface) {}

  Techstorm::PhysicsEngine::BodyInterfaceHolder::~BodyInterfaceHolder() {}
