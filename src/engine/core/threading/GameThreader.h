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


		virtual void setMaxThreadCount(const uint8_t threadCount) = 0;
		virtual uint8_t getMaxThreadCount() = 0;

		//virtual void enableSafetyCheck(const uint16_t safetyCheck) = 0;
		//virtual void disableSafetyCheck(const uint16_t safetyCheck) = 0;

		//virtual bool isSafetyCheckEnabled(const uint16_t safetyCheck) = 0;

		virtual void addScheduler(std::string const& schedulerName, uint8_t numThreads) = 0;
		
		virtual void removeScheduler(std::string const& schedulerName) = 0;

		virtual void init(uint8_t threadCount) = 0;

		virtual void cleanup() = 0;

		virtual std::unique_ptr<GameThreadScheduler>& getScheduler(std::string const& schedulerName) = 0;
	};

	class GameThreader : public IGameThreader {
	private:


		uint8_t mMaxThreadCount = 0;
		uint8_t mThreadCount = 0;

		// This is updated whenever a new thread is added or removed
		bool mHasReachedMaxThreadCount = false;
	public:

		enum class SafetyCheck : uint8_t
		{
			ThreadLimits = 0,
			HardwareLimits
		};
		std::unordered_map<std::string, std::unique_ptr<GameThreadScheduler>> mThreadSchedulerMap;

		std::unique_ptr<GameThreadScheduler> mThreadScheduler;

		explicit GameThreader() = default;
		explicit GameThreader(uint8_t threadCount) : mThreadCount(threadCount) {}

		void setMaxThreadCount(const uint8_t threadCount) override;
		uint8_t getMaxThreadCount() override;

		void addScheduler(std::string const& schedulerName, uint8_t numThreads) override;

		void removeScheduler(std::string const& schedulerName) override;

		void init(uint8_t threadCount) override;
		void init();

		void cleanup() override;

		std::unique_ptr<GameThreadScheduler>& getScheduler(std::string const& schedulerName) override;
	};
}
