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
#include <string>

#include "Core.h"
#include "MemoryAllocator.h"

#include "../graphics/Renderer.h"
#include "../graphics/window/Window.h"

#include "threading/GameThreader.h"
#include "threading/ThreadSafeVariable.h"

#include "Module.h"

namespace Atlas {
	struct AtlasSettings {
		AbstractMemoryAllocator* memoryAllocator;

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
	 * 
	 * @sa @ref IAtlasEngine
	 */
	class AtlasEngine : public IAtlasEngine {
	private:
		/**
		 * @brief .
		 *
		 * @since v
		 */
		volatile bool mShouldIdlebeforeRunning = true;

		/**
		 * @brief .
		 * 
		 * @since v
		 * 
		 * @todo Explicitly catch what ever is needed instead of just using & to capture it all
		 * when scheduling the @ref runRenderer function task.
		 * 
		 * @todo Replace the assert calls with Atlas' own assert function
		 */
		void initWithThreading();

		/**
		 * @brief .
		 *
		 * @since v
		 */
		void initWithoutThreading();

		/**
		 * @brief .
		 *
		 * @since v
		 */
		void runRenderer();
	public:


		using IAtlasEngine::IAtlasEngine;

		/**
		 * @brief As the name suggests, this function initializes the engine. If this module is
		 * infact threaded, it will call the initWithThreading function. If not, then it will
		 * call the initWithoutThreading function.
		 * 
		 * @since v0.0.1
		 */
		void init() override;

		/**
		 * @brief .
		 * 
		 * @since v
		 */
		void run() override;

		/**
		 * @brief .
		 *
		 * @since v
		 */
		void cleanup() override;
	};
}