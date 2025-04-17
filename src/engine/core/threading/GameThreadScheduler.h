#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <atomic>

#include "GameThread.h"
#include <stdexcept>
#include <type_traits>

namespace Atlas {


    class GameThreadScheduler {
    private:
        size_t mThreadCount;

        using Task = std::function<void()>;
        std::vector<thread> workers;
        std::queue<Task> taskQueue;

        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> running;

        bool isTaskQueueEmpty() const { 
            return taskQueue.empty(); 
        }

    public:
        // Disable copy
        GameThreadScheduler(const GameThreadScheduler&) = delete;

        GameThreadScheduler() = default;
        explicit GameThreadScheduler(size_t numThreads) : mThreadCount(numThreads) {
        }


        ~GameThreadScheduler() {
           {
                std::lock_guard<std::mutex> lock(queueMutex);
                running = false;
           }
            
            condition.notify_all();

            joinAll();
        }

        void init(size_t numThreads) {
            running = true;

            for (size_t i = 0; i < numThreads; ++i) {

                workers.emplace_back([this]() {

                    while (true) {
                        Task task;
                        {
                            std::unique_lock<std::mutex> lock(queueMutex);

                            condition.wait(lock, [this]() {
                                return !isTaskQueueEmpty() || !running;
                            });

                            if (!running && isTaskQueueEmpty())
                                return;

                            task = std::move(taskQueue.front());
                            taskQueue.pop();
                        }

                        task(); // Run task
                    }
                });
            }
        }

        // Template task scheduler with future return
        template<typename Func, typename... Args>
        auto schedule(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {

            using ReturnType = std::invoke_result_t<Func, Args...>;

            auto taskPtr = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );

            std::function<void()> wrapper = [taskPtr]() { (*taskPtr)(); };

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                if (!running)
                    throw std::runtime_error("Scheduler is shutting down, cannot schedule new tasks.");
                taskQueue.emplace(std::move(wrapper));
            }

            condition.notify_one();
            return taskPtr->get_future();
        }

        void joinAll() {
            for (thread& t : workers) {
                if (t.joinable())
                    t.join();
            }
        }

        bool isRunning() const {
			return running;
		}

        size_t getThreadCount() const {
			return mThreadCount;
		}

        GameThreadScheduler& operator=(const GameThreadScheduler&) = delete;
    };

}
