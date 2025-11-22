#pragma once

#include <cstdint>
#include <chrono>

#include "MemoryAllocator.h"

#include "threading/GameThreader.h"
#include "../graphics/Renderer.h"

namespace Atlas {


	class GameTimeManager {
	private:
		/**
		 * @brief How much time (in milliseconds) should pass between each update.
		 * 
		 * @since v
		 */
		std::chrono::milliseconds mUpdateInterval;

	public:

		GameTimeManager(std::chrono::milliseconds updateInterval) : mUpdateInterval(updateInterval) {}

		static inline std::chrono::time_point<std::chrono::steady_clock> GetCurrentTime() { return std::chrono::steady_clock::now(); }

		std::chrono::milliseconds getUpdateIntervalInMS() {}

		void setUpdateIntervalInMS(std::chrono::milliseconds updateInterval) {}

		virtual void update(std::chrono::milliseconds deltaTimeInMS) {}

	};
	


	class IAtlasEngine {
	public:
		struct Options {
			
		};

		//protected:
		//	AtlasSettings mEngineSettings;

		//public:
		//	ARenderer* mGameRenderingModule;
		//	IGameThreader* mGameThreader;

		//	AbstractMemoryAllocator* memoryAllocator;

		//	bool isThreaded = true;

			//explicit IAtlasEngine(EngineModulesInfo const& modulesInfo, AtlasSettings const& settings)
			//	: mEngineSettings(settings), mGameRenderingModule(modulesInfo.gameRenderingModule), mGameThreader(modulesInfo.gameThreader)
			//{
			//}

		virtual void setRenderer(std::shared_ptr<ARenderer> renderer) = 0;

		virtual void setGameThreader(std::shared_ptr <IGameThreader> gameThreader) = 0;

		//virtual void setMemoryAllocator(std::shared_ptr<AbstractMemoryAllocator> allocator) = 0;

		virtual void threadEngine(const int8_t cAllowedThreadCount) = 0;

		virtual void init() = 0;

		virtual void run() = 0;

		virtual void update() = 0;

		virtual void cleanup() = 0;

		virtual void exit(const uint32_t exitCode) = 0;

		virtual bool isThreaded() const = 0;

		virtual bool shouldExit() const = 0;
	};
}
