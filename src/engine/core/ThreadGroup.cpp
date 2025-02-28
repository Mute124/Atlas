#include "ThreadGroup.h"
#include "GameThread.h"
#include <exception>
#include <functional>
#include <mutex>
#include <string>
#include <type_traits>

Atlas::ThreadGroup::~ThreadGroup() {
    stopAll();
}

void Atlas::ThreadGroup::addThread(const std::string& name, std::function<void()> task) {
    std::unique_lock<std::mutex> lock(mMutex);
    GameThread t = GameThread(name, task);
    t.detach();
    mThreads.push_back(std::move(t));
}

void Atlas::ThreadGroup::stopAll() {
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!mThreads.empty()) {
            mShouldStop = true;
            mConditionalVariable.notify_all();
        }
    }
    for (auto& t : mThreads) {
        try {
            t.join();
        }
        catch (const std::exception& e) {
            // Handle exception or log it
        }
    }
}

void Atlas::ThreadGroup::waitForAll() {
    std::unique_lock<std::mutex> lock(mMutex);
    if (!mThreads.empty()) {
        mConditionalVariable.wait(lock, [this] { return mThreads.empty(); });
    }
}
