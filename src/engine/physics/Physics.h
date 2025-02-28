/*****************************************************************//**
 * @file   Physics.h
 * 
 * @brief  This file contains all the required components for 
 * Atlas's physics engine. As of right now, it is built on 
 * JoltPhysics and there may be plans to change and/or add support
 * for another physics engine in the future. 
 * 
 * @important The physics engine is not yet fully implemented and 
 * is subject to change. Please expect the API to change in the
 * future, especially since the addition of other physics engines
 * is being considered.
 * 
 * @note This is a work in progress, so it is not very extensible.
 * With that being said, it is able to do its job but it is not as 
 * flexible as it could be.
 * 
 * @date   February 2025
 * 
 * @since v0.0.3
 *********************************************************************/
#pragma once

/**
 * @brief Enables support for JoltPhysics. As is stated in the 
 * header documentation, this is being declared here because
 * Atlas only supports JoltPhysics at the moment. However,
 * this may change in the future, so the prospect of it
 * is enough to justify declaring it here.
 * 
 * @since v0.0.3
 */
#define ATLAS_ENABLE_JOLTPHYSICS

/******************************************************************
* JoltPhysics includes
********************************************************************/

// As is documented in Jolt's documentation, this include is required
// because the other includes do not include this file.
#include <Jolt/Jolt.h> 

#include <Jolt/Core/Core.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/IssueReporting.h>
#include <Jolt/Core/JobSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>

/******************************************************************
* Atlas includes
********************************************************************/
#include "../Common.h"
#include "Layers.h"
#include "Listeners.h"


// Shut the compiler up. 
JPH_SUPPRESS_WARNINGS

using namespace JPH::literals;


namespace Atlas {
	/**
	 * @brief A struct that contains data on what resources should be
	 * allocated for the physics engine.
	 * 
	 * @remarks T
	 * 
	 * @since 
	 */
	struct AllocatedPhysicsResources {		

		JPH::uint cMaxBodies = 65536;

		JPH::uint cNumBodyMutexes = 0;

		JPH::uint cMaxBodyPairs = 65536;

		JPH::uint cMaxContactConstraints = 1024;

		int preAllocatedMemory = 20 * 1024 * 1024;

		AllocatedPhysicsResources();
		
		AllocatedPhysicsResources(AllocatedPhysicsResources const& resources);
				
		AllocatedPhysicsResources(const JPH::uint& cMaxBodies, const JPH::uint& cNumBodyMutexes, const JPH::uint& cMaxBodyPairs, const JPH::uint& cMaxContactConstraints);
	};
	
	/// <summary>
	/// Handles everything related to Atlas' physics engine.
	/// </summary>
	/// <seealso cref="Atlas::Singleton&lt;PhysicsEngine&gt;" />
	/// <seealso cref="Atlas::Singleton&lt;PhysicsEngine&gt;" />
	class PhysicsEngine : public Atlas::Singleton<PhysicsEngine> {
	private:
		/**
		 * @brief Since the body interface reference was required as a
		 * member variable and it could not be created in the constructor
		 * because the physics system was not initialized yet, the
		 * BodyInterfaceHolder class was created to essentially wrap the
		 * JPH::BodyInterface. 
		 * 
		 * @since 
		 */
		class BodyInterfaceHolder {
		public:
			JPH::BodyInterface& bodyInterface;

			explicit BodyInterfaceHolder(JPH::BodyInterface& bodyInterface);
			~BodyInterfaceHolder();
		};
		
		/**
		 * @brief Jolt physics's collision steps argument.
		 * 
		 * @since v0.0.3
		 */
		int	mCollisionSteps = 1;

		
		BodyInterfaceHolder* mBodyInterfaceHolder;

		/**
		 * @brief The m physics system
		 */
		JPH::PhysicsSystem* mPhysicsSystem = nullptr;

		/**
		 * @brief Whether or not the physics engine has been initialized
		 */
		bool mIsInitialized = false;

		/// <summary>
		/// The m resources
		/// </summary>
		AllocatedPhysicsResources mResources;

		/// <summary>
		/// The m temporary allocator
		/// </summary>
		JPH::TempAllocator* mTempAllocator = nullptr;

		/// <summary>
		/// The m job system thread pool
		/// </summary>
		JPH::JobSystem* mJobSystemThreadPool = nullptr;

		/// <summary>
		/// The m broad phase layer interface
		/// </summary>
		BPLayerInterfaceImpl mBroadPhaseLayerInterface;

		/// <summary>
		/// The m object vs broad phase layer filter
		/// </summary>
		ObjectVsBroadPhaseLayerFilterImpl mObjectVsBroadPhaseLayerFilter;

		/// <summary>
		/// The m object layer pair filter
		/// </summary>
		ObjectLayerPairFilterImpl mObjectLayerPairFilter;

		/// <summary>
		/// The contact listener
		/// </summary>
		/// <remarks>
		/// This is used to listen in on the contact events. For more information, please refer to Jolt's JPH::ContactListener class.
		/// </remarks>
		JPH::ContactListener* mContactListener = nullptr;

		/// <summary>
		/// The m physics settings
		/// </summary>
		/// <seealso cref="JPH::PhysicsSettings" />
		JPH::PhysicsSettings  mPhysicsSettings;

		/// <summary>
		/// The jolt body activation listener class.
		/// </summary>
		/// <remarks>
		/// This is used to listen in on the activation and deactivation of physics bodies. For more information, please refer to Jolt's JPH::BodyActivationListener class.
		/// </remarks>
		/// <seealso cref="JPH::BodyActivationListener" />
		JPH::BodyActivationListener* mBodyActivationListener = nullptr;

	public:
		PhysicsEngine() = default;
		
		/// <summary>
		/// Initializes the physics engine with the resources (<see cref="AllocatedPhysicsResources"/>). The reason for the struct is to keep things organized and have as little parameter hell as
		/// possible.
		/// </summary> 
		/// <param name="resources">The resources.</param>
		void init(const AllocatedPhysicsResources resources);
		
		/// <summary>
		/// Updates the physics engine.
		/// \note The delta time is used for Jolt's JPH::PhysicsSystem::Update function and is in seconds. This function is already handled internally (assuming you are using the default systems).
		/// If you need to update the physics engine yourself, you can do that
		/// </summary>
		/// <param name="cDeltaTime">A delta time constant that will be passed to Jolt's JPH::PhysicsSystem::Update function.</param>
		void update(const float cDeltaTime);
		
		/// <summary>
		/// Optimizes the broad phase. This is simply a wrapper for Jolt's JPH::PhysicsSystem::OptimizeBroadPhase function, but it is provided here for convenience. 
		/// \note Please note that this should NOT be called often because it is expensive, so only run it after adding alot of physics objects. Even then, calling this only once is usually enough.
		/// </summary>
		void optimizeBroadPhase();
		
		/// <summary>
		/// Gets the body interface by returning the internal body interface reference (<see cref="BodyInterfaceHolder"/>). Use this if you need a reference to the body interface to add bodies or 
		/// something else.
		/// </summary>
		/// <returns>A reference to the body interface</returns>
		JPH::BodyInterface& getBodyInterface() const;

	
	};

	/**
	 * @brief This is the already provided trace function for the
	 * physics engine.
	 *
	 * @note Keep in mind this is used exclusively by the
	 * physics engine, so there is not much use besides that.
	 *
	 * @todo TODO: Implement this more properly
	 *
	 * @param inFMT The format string
	 *
	 * @param vaArgs Variable arguments for the trace. This is
	 * declared as "...", which means it is a variable number of arguments.
	 */
	static void DefaultPhysicsTrace(const char* inFMT, ...);

#ifdef JPH_ENABLE_ASSERTS

	/**
	 * @brief This is the already provided failed assertion
	 * function.
	 *
	 * @remarks Keep in mind this is used exclusively by the
	 * physics engine, so there is not much use besides that.
	 *
	 * @todo Implement this more properly
	 *
	 * @param inExpression The assert expression that failed.
	 *
	 * @param inMessage The assertion message.
	 *
	 * @param inFile The file that the assert was in.
	 *
	 * @param inLine The line that the assert was on.
	 *
	 * @return
	 */
	static bool DefaultPhysicsAssertFailed(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine);

#endif // JPH_ENABLE_ASSERTS
}
