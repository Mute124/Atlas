#include "GameThreadScheduler.h"

bool Atlas::GameThreadScheduler::isTaskQueueEmpty() const {
    return mTaskQueue.empty();
}

Atlas::GameThreadScheduler::GameThreadScheduler(uint8_t numThreads) : mThreadCount(numThreads) {
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
