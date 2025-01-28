/// @file Physics.h
/// @brief This file contains Atlas's physics engine and is built on JoltPhysics. 
/// @note This is a work in progress, so it is not very extensible. With that being said, it is able to do its job but it is not as flexible as it could be.
#pragma once

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

#include "../Common.h"
#include "Layers.h"
#include "Listeners.h"

// Shut the compiler up. 
JPH_SUPPRESS_WARNINGS

using namespace JPH::literals;

namespace Atlas {

	/// <summary>
	/// This is the already provided trace function for the physics engine. 
	/// @note Keep in mind this is used exclusively by the physics engine, so there is not much use besides that. 
	/// @todo TODO: Implement this more properly
	/// </summary>
	/// <param name="inFMT">The format to print.</param>
	/// <param name="">Variable arguments for the trace.</param>
	static void DefaultPhysicsTrace(const char* inFMT, ...)
	{
		// TODO: See above documentation
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		// Print to the TTY
		cout << buffer << endl;
	}

#ifdef JPH_ENABLE_ASSERTS
	
	/// <summary>
	/// This is the already provided failed assertion function. Keep in mind this is used exclusively by the physics engine, so there is not much use besides that. 
	/// @todo Implement this more properly
	/// </summary>
	/// <param name="inExpression">The assert expression that failed.</param>
	/// <param name="inMessage">The assertion message.</param>
	/// <param name="inFile">The file that the assert was in.</param>
	/// <param name="inLine">The line that the assert was on.</param>
	/// <returns></returns>
	static bool DefaultPhysicsAssertFailed(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// TODO: See above documentation
		// Print to the TTY
		cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << endl;

		// Breakpoint
		return true;
	};

#endif // JPH_ENABLE_ASSERTS
	
	/// <summary>
	/// This struct contains the allocated resources for the physics engine and is used to initialize the physics engine.
	/// </summary>
	struct AllocatedPhysicsResources {		
		/// <summary>
		/// The c maximum bodies
		/// </summary>
		JPH::uint cMaxBodies = 65536;
		
		/// <summary>
		/// The c number body mutexes
		/// </summary>
		JPH::uint cNumBodyMutexes = 0;
		
		/// <summary>
		/// The c maximum body pairs
		/// </summary>
		JPH::uint cMaxBodyPairs = 65536;
		
		/// <summary>
		/// The c maximum contact constraints
		/// </summary>
		JPH::uint cMaxContactConstraints = 1024;
		
		/// <summary>
		/// The pre allocated memory
		/// </summary>
		int preAllocatedMemory = 20 * 1024 * 1024;
				
		/// <summary>
		/// Initializes a new instance of the <see cref="AllocatedPhysicsResources"/> struct.
		/// </summary>
		AllocatedPhysicsResources();
		
		/// <summary>
		/// Initializes a new instance of the <see cref="AllocatedPhysicsResources"/> struct.
		/// </summary>
		/// <param name="resources">A constant reference to an instance of the <see cref="AllocatedPhysicsResources"/> struct.</param>
		AllocatedPhysicsResources(AllocatedPhysicsResources const& resources);
				
		/// <summary>
		/// Initializes a new instance of the <see cref="AllocatedPhysicsResources"/> struct.
		/// </summary>
		/// <param name="cMaxBodies">A constant that specifies the maximum bodies.</param>
		/// <param name="cNumBodyMutexes">A constant that specifies the number body mutexes.</param>
		/// <param name="cMaxBodyPairs">A constant that specifies the maximum body pairs.</param>
		/// <param name="cMaxContactConstraints">A constant that specifies the maximum contact constraints.</param>
		AllocatedPhysicsResources(const JPH::uint& cMaxBodies, const JPH::uint& cNumBodyMutexes, const JPH::uint& cMaxBodyPairs, const JPH::uint& cMaxContactConstraints);
	};
	
	/// <summary>
	/// Handles everything related to Atlas' physics engine.
	/// </summary>
	/// <seealso cref="Atlas::Singleton&lt;PhysicsEngine&gt;" />
	/// <seealso cref="Atlas::Singleton&lt;PhysicsEngine&gt;" />
	class PhysicsEngine : public Atlas::Singleton<PhysicsEngine> {
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

	private:
				
		/// <summary>
		/// Since the body interface reference was required as a member variable and it could not be created in the constructor, the BodyInterfaceHolder class was created to essentially wrap around
		/// the JPH::BodyInterface. 
		/// </summary>
		class BodyInterfaceHolder {
		public:
			JPH::BodyInterface& bodyInterface;

			explicit BodyInterfaceHolder(JPH::BodyInterface& bodyInterface);
			~BodyInterfaceHolder();
		};
		
		/// <summary>
		/// The m collision steps
		/// </summary>
		int	mCollisionSteps = 1;
		
		//
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
	};
}
