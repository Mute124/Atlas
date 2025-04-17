/**************************************************************************************************
 * @file AtlasEngine.h
 * 
 * @brief This file contains the declaration of the @ref AtlasEngine class.
 * 
 * @date April 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Core.h"
#include "MemoryAllocator.h"

#include "../renderer/Renderer.h"
#include "../renderer/window/Window.h"

#include "threading/GameThreader.h"
#include "threading/ThreadSafeVariable.h"

#include "Module.h"

namespace Atlas {
	struct AtlasSettings {
		MemoryAllocator* memoryAllocator;

		bool isThreaded = true;
	};


	struct EngineModulesInfo {
		IRenderer* gameRenderingModule;
		IGameThreader* gameThreader;
	};

	class IAtlasEngine {
	protected:
		AtlasSettings mEngineSettings;

	public:
		IRenderer* mGameRenderingModule;
		IGameThreader* mGameThreader;

		explicit IAtlasEngine(EngineModulesInfo const& modulesInfo, AtlasSettings const& settings);

		virtual void init() = 0;

		virtual void run() = 0;

		virtual void cleanup() = 0;
	};

	/**
	 * @brief This is the core and heart of Atlas; it is responsible for just
	 * about everything that is required to run the engine. It contains multiple
	 * different modules that can be used to extend the engine. If you want to
	 * extend the engine, you can simply inherit the respective module and utilize
	 * the concepts of polymorphism to your advantage. Keep in mind that the modules
	 * are stored as a ThreadSafeVariable and can be used concurrently. That means
	 * the user can multi-thread these modules if they so choose. This is just a
	 * default implementation of the IAtlasEngine and you do not have to use this
	 * class. However, if you would like to use a different implementation, you can
	 * simply inherit from IAtlasEngine and override the virtual functions. But one thing 
	 * must remain: the class must inherit from IAtlasEngine or be a child of IAtlasEngine.
	 * 
	 * @note There should not be more than one instance of this object. 
	 * 
	 * @since v0.0.1
	 */
	class AtlasEngine : public IAtlasEngine {
	private:
		volatile bool mShouldIdlebeforeRunning = true;

		void initWithThreading();

		void initWithoutThreading();

		void runRenderer();
	public:

		using IAtlasEngine::IAtlasEngine;

		void init() override;

		void run() override;

		void cleanup() override;
	};
}