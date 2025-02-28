#pragma once
#include <string>
#include <functional>
#include <thread>
namespace Atlas {
	class GameThread {
	private:
		std::string mThreadName;
		std::jthread mThread;
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
			mThread = std::jthread([this]() {
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
}