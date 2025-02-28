#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>


namespace Atlas {


	// Layer that objects can be in, determines which other objects it can collide with
	// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
	// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
	// but only if you do collision testing).
	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer TERRIAN = 2;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 3;
	};

	// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
	// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
	// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
	// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr JPH::uint NUM_LAYERS(2);
	};

	/// Class that determines if two object layers can collide
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:		
		/// <summary>
		/// This is a function that will decide if two object layers can collide. The layers that this uses is the namespace called: Layers. 
		/// </summary>
		/// <param name="inObject1">The layer of the first object.</param>
		/// <param name="inObject2">The layer of the second object.</param>
		/// <returns></returns>
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="BPLayerInterfaceImpl"/> class.
		/// </summary>
		BPLayerInterfaceImpl();
				
		/// <summary>
		/// Gets the number of broad phase layers. This is a virtual function, so feel free to override it and suit it to your needs.
		/// </summary>
		/// <returns>A JPH::uint that denotes the number of broad phase layers.</returns>
		virtual JPH::uint GetNumBroadPhaseLayers() const override;
		
		/// <summary>
		/// Gets the broad phase layer.
		/// </summary>
		/// <param name="inLayer">The in layer.</param>
		/// <returns></returns>
		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};

}