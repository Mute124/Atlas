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

    class GameThread {
    private:
        std::string mThreadName;
        std::thread mThread;
        std::function<void()> mTask;
    public:

		GameThread() = default;
        
		GameThread(std::string name, std::function<void()> task) : mTask(task) {
			mThreadName = name;
		}

        void execute() {
            return mTask();
        }

        void start() {
			mThread = std::thread([this]() {
				execute();
			});
		}


        void join() {
			mThread.join();
		}

        void detach() {
			mThread.detach();
		}

        void setTask(std::function<void()> task) {
			mTask = task;
		}


    };

    class ThreadGroup {
    public:
        ThreadGroup() = default;
        ~ThreadGroup() {
            stopAll();
        }

        void addThread(const std::string& name, std::function<void()> task) {
            std::unique_lock<std::mutex> lock(mMutex);
            GameThread t = GameThread(name, task);
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
                try {
                    t.join();
                }
                catch (const std::exception& e) {
                    // Handle exception or log it
                }
            }
        }

        void waitForAll() {
            std::unique_lock<std::mutex> lock(mMutex);
            if (!mThreads.empty()) {
                mConditionalVariable.wait(lock, [this] { return mThreads.empty(); });
            }
        }

    private:
        std::vector<GameThread> mThreads;
        bool mShouldStop = false;
        std::mutex mMutex;
        std::condition_variable mConditionalVariable;
        std::string mGroupName;
    };
}