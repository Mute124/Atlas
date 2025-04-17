#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "GameThreadScheduler.h"
#include "GameThread.h"

#include "../Module.h"


namespace Atlas {
	class IGameThreader : public EngineModule {
	public:

		virtual void addScheduler(std::string const& schedulerName, uint8_t numThreads) = 0;
		
		virtual void removeScheduler(std::string const& schedulerName) = 0;

		virtual void init(uint8_t threadCount) = 0;

		virtual void cleanup() = 0;

		virtual std::unique_ptr<GameThreadScheduler>& getScheduler(std::string const& schedulerName) = 0;
	};

	class GameThreader : public IGameThreader {
	private:
		uint8_t mThreadCount = 0;


	public:
		std::unordered_map<std::string, std::unique_ptr<GameThreadScheduler>> mThreadSchedulerMap;

		std::unique_ptr<GameThreadScheduler> mThreadScheduler;

		explicit GameThreader() = default;
		explicit GameThreader(uint8_t threadCount) : mThreadCount(threadCount) {}

		void addScheduler(std::string const& schedulerName, uint8_t numThreads) override;

		void removeScheduler(std::string const& schedulerName) override;

		void init(uint8_t threadCount) override;
		void init();

		void cleanup() override;

		std::unique_ptr<GameThreadScheduler>& getScheduler(std::string const& schedulerName) override;
	};
}
