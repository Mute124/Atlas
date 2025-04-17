#pragma once

#include <thread>
#include <memory>
#include <string>
#include <functional>
#include <type_traits>
#include <optional>
#include <atomic>
#include <cstdint>
#include <vector>

#include "../Common.h"



namespace Atlas {
#ifndef ATLAS_ENABLE_THREADING

	template<typename T>
	concept ThreadTypeConcept = std::is_same_v<T, std::thread> || std::is_same_v<T, std::jthread>;

	struct GameThreadingStats {
		uint8_t totalActiveThreads; 
	};

	struct ThreadLaunchInfo {
		bool bDetach;
		std::string threadName;
		std::function<void()> threadFunction;
	};


	template<ThreadTypeConcept T_THREAD_TYPE>
	class IGameThread {
	protected:
		friend class GameThreadScheduler;

	public:

		using ThreadType = T_THREAD_TYPE;

		IGameThread() = default;

		virtual ~IGameThread() = default;

		/**
		 * @brief A constant representing the number of hardware threads available on the system
		 */
		static inline const unsigned int sHardwareConcurrency = ThreadType::hardware_concurrency();
		
		ThreadType mThread;

		virtual bool isIdle() = 0;
		virtual bool isDetached() = 0;
		virtual bool isReady() = 0;

		virtual bool isJoinable() = 0;

		virtual void detach() = 0;
		virtual void join() = 0;
		virtual void execute() = 0;
		


	};

	// TODO: This
	class GameThread : public IGameThread<std::jthread> {
	private:
		const std::function<void()> mThreadFunction;

		std::atomic<bool> mIsIdle;
		std::atomic<bool> mIsReady;

		void setIdleState(bool newState);

		void setIsReady(bool newReadinessState);

	public:

		explicit GameThread(ThreadLaunchInfo const& launchInfo);

		bool isIdle() override;

		bool isDetached() override;

		bool isReady() override;

		bool isJoinable() override;

		void detach() override;

		void join() override;

		virtual void execute() override;

		
	};





#endif
}