/**************************************************************************************************
 * @file GameThreadScheduler.cpp
 * 
 * @brief This file contains the implementation of the @ref GameThreadScheduler class.
 * 
 * @date April 2025
 * 
 * @since v0.0.10
 ***************************************************************************************************/
#include <cstdint>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "GameThreadScheduler.h"


bool Atlas::GameThreadScheduler::isTaskQueueEmpty() const {
    return mTaskQueue.empty();
}

void Atlas::GameThreadScheduler::setStatus(EGameThreadSchedulerStatus status)
{
    mStatus = status;
}

Atlas::GameThreadScheduler::GameThreadScheduler(uint8_t numThreads, bool callInit) : mThreadCount(numThreads) {
    if (callInit)
    {
		init(numThreads);
    }
}

Atlas::GameThreadScheduler::~GameThreadScheduler() {
    {
        std::lock_guard<std::mutex> lock(mQueueMutex);
        mIsRunning = false;
    }

    mWorkAvailableConditional.notify_all();

    joinAll();
}

void Atlas::GameThreadScheduler::init(uint8_t numThreads) {
    mIsRunning = true;
    setStatus(EGameThreadSchedulerStatus::Initializing);

    for (size_t i = 0; i < numThreads; ++i) {

        mWorkersVector.emplace_back([this]() {

            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(mQueueMutex);

                    mWorkAvailableConditional.wait(lock, [this]() {
                        return !isTaskQueueEmpty() || !mIsRunning;
                        });

                    if (!mIsRunning && isTaskQueueEmpty())
                        return;

                    task = std::move(mTaskQueue.front());
                    mTaskQueue.pop();
                }

                task(); // Run task
            }
            });
    }

	setStatus(EGameThreadSchedulerStatus::Running);
}

void Atlas::GameThreadScheduler::joinAll() {
    for (std::thread& workerThread : mWorkersVector) {
        if (workerThread.joinable())
            workerThread.join();
    }
}

bool Atlas::GameThreadScheduler::isRunning() const {
    return mIsRunning;
}

uint8_t Atlas::GameThreadScheduler::getThreadCount() const {
    return mThreadCount;
}
