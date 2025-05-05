/**************************************************************************************************
 * @file GameThreadScheduler.h
 * 
 * @brief Scheduler for running tasks on separate threads.
 * 
 * @date April 2025
 * 
 * @since v0.0.10
 ***************************************************************************************************/
#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <type_traits>
#include <cstdint>

#include "GameThread.h"


namespace Atlas {

    /**
     * @brief The state of the scheduler.
     * 
     * @remarks This enum class is useful for doing diagnostics on the scheduler. 
     * 
     * @since v0.0.10
     */
    enum class EGameThreadSchedulerStatus : uint8_t {
		/**
		 * @brief The state of the scheduler is unknown. Typically this means that the scheduler is not initialized or something is wrong.
		 * 
		 * @since v0.0.10
		 */
        Unknown = 0,

        /**
         * @brief The scheduler is in the process of initializing.
         * 
         * @since v0.0.10
         */
        Initializing,

		/**
		 * @brief The scheduler is running. This does not mean that there is work to be done, just that the scheduler is running.
		 * 
		 * @since v0.0.10
		 */
        Running,

        /**
         * @brief The scheduler is stopped. This implies that the scheduler was once initialized and running, but is now stopped.
         * 
         * @since v0.0.10
         */
		Stopped,

        /**
         * @brief The scheduler is shutting down.
         * 
         * @since v0.0.10
         */
		ShuttingDown
	};
    
    /**
     * @brief Scheduler for running tasks on separate threads. It will take given tasks, and distribute them across threads.
     * 
     * @pre Atlas must have multi-threading enabled for this to even be used.
     * 
     * @note Everything is thread-safe.
     * 
     * @note This class @b CANNOT be copied.
     * 
     * @since v0.0.10
     * 
     * @todo Refactor this so that it works with @ref Atlas::GameThread instead of std::thread. See @ref mWorkersVector for the todo comment
     */
    class GameThreadScheduler {
    private:
        /**
         * @brief A task to be executed.
         *
         * @since v0.0.10
         */
        using Task = std::function<void()>;

        /**
         * @brief The current status of the scheduler.
         * 
         * @since v0.0.10
         */
        EGameThreadSchedulerStatus mStatus = EGameThreadSchedulerStatus::Unknown;

        /**
         * @brief Number of threads in this scheduler.
         * 
         * @since v0.0.10
         */
        uint8_t mThreadCount;
        
        /**
         * @brief A vector that contains all the worker threads. 
         * 
         * @note The size of this vector is equal to mThreadCount. If not, something is wrong.
         * 
         * @todo Instead of using std::thread, implement this so that it works with @ref Atlas::GameThread
         * 
         * @since v0.0.10
         */
        std::vector<std::thread> mWorkersVector;

        /**
         * @brief A queue of tasks assigned to this scheduler. All tasks are executed in the order they are added to the queue. Additionally, the threads get their tasks from this queue.
         * 
         * @since v0.0.10
         * 
         * @sa @ref mQueueMutex The mutex that is used to make this object thread-safe.
         * 
         * @sa @ref mWorkAvailableConditional The way threads know if there are tasks in the queue available.
         */
        std::queue<Task> mTaskQueue;

        /**
         * @brief The mutex that is used to the tasks queue to be thread-safe.
         * 
         * @since v0.0.10
         */
        std::mutex mQueueMutex;

        /**
         * @brief Signals to workers that there are tasks available in the task queue.
         * 
         * @since v0.0.10
         */
        std::condition_variable mWorkAvailableConditional;

        /**
         * @brief Whether or not this scheduler is running. This does not necessarily mean that there are tasks being executed or in the queue.
         * 
         * @since v0.0.10
         */
        std::atomic<bool> mIsRunning;

        /**
		 * @brief Returns whether or not the task queue is empty.
		 * 
		 * @since v0.0.10
		 */
        bool isTaskQueueEmpty() const {
            return mTaskQueue.empty();
        }

        void setStatus(EGameThreadSchedulerStatus status)
        {
            mStatus = status;
        }

    public:
        // Disable copy
        GameThreadScheduler(const GameThreadScheduler&) = delete;

        /**
         * @brief Default constructor.
         * 
         * @note This constructor is simply defined by = default because there is nothing that can be done without knowing how many threads to create.
         *
         * @since v0.0.10
         */
        GameThreadScheduler() = default;

        /**
         * @brief Initializes this class with the specified amount of threads.
         * 
         * @param numThreads How many worker threads should be made?
         * 
         * @todo Call the @ref init function here. Also, add a boolean parameter that the user can use to define whether or not they want to call said function
         * 
         * @since v0.0.10
         */
        explicit GameThreadScheduler(uint8_t numThreads, bool callInit = true) : mThreadCount(numThreads) {
            if(callInit)
            {
                init(numThreads);
            }
        }

        /**
         * @brief The default deconstructor that joins all threads (if they are joinable) and cleans up everything.
         * 
         * @todo Check to see if this completes the rest of the tasks before shutting down. I suspect that it will not complete the tasks in the queue, and this is something that must be fixed if it is true.
         * 
         * @since v0.0.10
         */
        ~GameThreadScheduler() {
            {
                std::lock_guard<std::mutex> lock(mQueueMutex);
                mIsRunning = false;
            }

            mWorkAvailableConditional.notify_all();

            joinAll();
        }

        /**
         * @brief Initializes this class with the specified amount of threads.
         * 
         * @note This must be called before this class can be used.
         * 
         * @param numThreads The amount of worker threads to create.
         * 
         * @since v0.0.10
         */
        void init(uint8_t numThreads) {
            mIsRunning = true;
            setStatus(EGameThreadSchedulerStatus::Initializing);

            for(size_t i = 0; i < numThreads; ++i) {

                mWorkersVector.emplace_back([this]() {

                    while(true) {
                        Task task;
                        {
                            std::unique_lock<std::mutex> lock(mQueueMutex);

                            mWorkAvailableConditional.wait(lock, [this]() {
                                return !isTaskQueueEmpty() || !mIsRunning;
                                });

                            if(!mIsRunning && isTaskQueueEmpty())
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

        /**
         * @brief Schedules a task to be executed. The templates are not really necessary, see this function's details section in the documentation for more information.
         * 
         * @details This function does not require you to use the templates. You can simply pass the function and the function's arguments when you call this function. For example, consider the following:
         * 
         * @code 
         *      scheduler.schedule(add, 5, 7);
		 * @endcode 
		 * 
		 * or 
		 * 
		 * @code 
		 *      scheduler.schedule([](int a, int b) { return a + b; }, 5, 7);
		 * @endcode
		 * 
		 * @pre For this function to work, the below must be true:
		 * 1. The function must be callable. This means that it must have a return type and it must have arguments.
		 * 2. Atlas must have multi-threading enabled
		 * 3. Atlas must be initialized
		 * 4. Atlas must not be shutting down or in the process of shutting down
		 * 5. This scheduler must be initialized
		 * 6. This scheduler must not be shutting down or in the process of shutting down
		 * 
		 * @throws std::runtime_error if any of the above conditions are not met
		 * 
         * @note This can be described as a template task scheduler with future return
         * 
         * @note This class is thread-safe
         * 
         * @param func The function to be executed. It can be any callable object
         * 
         * @param ...args The arguments to be passed to the function
         * 
         * @return A future that will contain the return value of the function
         * 
         * @since v0.0.10
         * 
         * @todo Review the code in this function to see where some checks need to be added to avoid problems.
         */
        template<typename Func, typename... Args>
        auto schedule(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {

            // Determine the return type of the callable object (Func) when invoked with the given arguments (Args...)
            using ReturnType = std::invoke_result_t<Func, Args...>;

            // Create a shared pointer to a packaged task that wraps the callable object (Func) and its arguments (Args...)
            // The packaged task is a callable object that can be executed later, and its return value can be retrieved using a future
            auto taskPtr = std::make_shared<std::packaged_task<ReturnType()>>(
                // Use std::bind to create a callable object that invokes the original callable object (Func) with the given arguments (Args...)
                // The std::forward function is used to preserve the value category (lvalue or rvalue) of the original arguments
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );

            // Create a wrapper function that executes the packaged task when called
            // This wrapper function is used to decouple the execution of the task from the scheduling mechanism
            std::function<void()> wrapper = [taskPtr]() { 
                (*taskPtr)(); 
            };

            // Acquire a lock on the scheduler's queue mutex to ensure thread-safe access to the queue
            {
                std::lock_guard<std::mutex> lock(mQueueMutex);

                // make sure that the scheduler is not shutting down
                if (!mIsRunning) {
                    throw std::runtime_error("Scheduler is shutting down, cannot schedule new tasks.");
                }

                // Forward the callable object (Func) and its arguments (Args...) to the packaged task.
                // In other words, finally put the callable object (Func) and its arguments (Args...) into the queue for execution
                mTaskQueue.emplace(std::move(wrapper));
            }

            // let worker threads know that there is a new task
            mWorkAvailableConditional.notify_one();

            return taskPtr->get_future();
        }

        /**
         * @brief Joins all worker threads in this scheduler if they are joinable.
         * 
         * @pre Atlas must have multi-threading enabled and this scheduler must have been initialized
         * 
         * @since v0.0.10
         */
        void joinAll() {
            for(std::thread& workerThread : mWorkersVector) {
                if(workerThread.joinable())
                    workerThread.join();
            }
        }

        /**
         * @brief Checks if this scheduler is running. The outcome of this function is based on the value of the mIsRunning member variable and does not
         * imply that there are tasks to be done, just that the scheduler is running and initialized.
         * 
         * @return @a true if the scheduler is running, @a false otherwise
         * 
         * @since v0.0.10
         */
        bool isRunning() const {
            return mIsRunning;
        }

        /**
         * @brief Returns the number of worker threads in this scheduler. 
         * 
         * @return An 8-bit unsigned integer representing the number of worker threads that this scheduler created on initialization.
         * 
         * @since v0.0.10
         */
        uint8_t getThreadCount() const {
            return mThreadCount;
        }

        GameThreadScheduler& operator=(const GameThreadScheduler&) = delete;
    };

}
