#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <functional>

namespace Atlas {

    class ThreadGroup {
    public:
        ThreadGroup() = default;
        ~ThreadGroup() {
            stopAll();
        }

        void addThread(std::function<void()> task) {
            std::unique_lock<std::mutex> lock(mMutex);
            std::thread t = std::thread(task);
            t.detach();
            mThreads.push_back(std::move(t));
        }

        void stopAll() {
            {
                std::unique_lock<std::mutex> lock(mMutex);
                if (!mThreads.empty()) {
                    mShouldStop = true;
                    mConditionalVariable.notify_all();
                }
            }
            for (auto& t : mThreads) {
                t.join();
            }
        }

        void waitForAll() {
            std::unique_lock<std::mutex> lock(mMutex);
            mConditionalVariable.wait(lock, [this] { return mThreads.empty(); });
        }

    private:
        std::vector<std::thread> mThreads;
        bool mShouldStop = false;
        std::mutex mMutex;
        std::condition_variable mConditionalVariable;
        std::string mGroupName;
    };

	template<typename T_USER_PROJECT>
	class IGameThread {
	public:
		//virtual static inline std::shared_ptr<T_USER_PROJECT> GetUserProject() = 0;

		virtual float getDeltaTime() = 0;
		virtual float getFrameTime() = 0;

		virtual volatile bool isIdle() = 0;
	};
}