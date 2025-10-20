/**************************************************************************************************
 * @file Timer.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "Timer.h"

Atlas::ATimer::~ATimer() { 
	ATimer::reset();
}

void Atlas::ATimer::start() { 
	start(mStartTime);
}

void Atlas::ATimer::reset() {
	if (mbClockRunning) {
		stop();
	}

	mStartTime = 0;

	mbClockRunning = false;
	mbClockPaused = false;
	mbClockFinished = false;
}

void Atlas::AsyncTimer::start(uint64_t length)
{
	ATimer::start();

	mTimerFuture = std::async(std::launch::async, &AsyncTimer::Wait, this);
}

void Atlas::AsyncTimer::start()
{
	start(mDuration);
}

void Atlas::AsyncTimer::stop()
{
}


