/**************************************************************************************************
 * @file Timer.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <chrono>
#include <type_traits>
#include <cstdint>
#include <future>

namespace Atlas {



	template<typename T_TRY_CLOCK_TYPE>
	concept Clock = std::chrono::is_clock<T_TRY_CLOCK_TYPE>::value;

	class ATimer {
	protected:
		uint64_t mStartTime = 0;

		bool mbClockRunning = false;
		bool mbClockPaused = false;
		bool mbClockFinished = false;

	public:

		explicit ATimer(uint64_t startTime) : mStartTime(startTime) {}

		ATimer() = default;

		virtual ~ATimer();

		virtual void start(uint64_t length) = 0;

		virtual void start();
	
		virtual void stop() = 0;

		virtual void reset();

	};

	class AsyncTimer : public ATimer {
	private:
		uint64_t mDuration = 0;

		std::future<void> mTimerFuture;

		static void Wait(AsyncTimer* timer) {
			std::this_thread::sleep_for(std::chrono::milliseconds(timer->mDuration));
			timer->mTimerFuture.get();
		}


	public:

		void start(uint64_t length) override;

		void start() override;

		void stop() override;
	};
}
