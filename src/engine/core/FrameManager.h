#pragma once
#include <memory>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <string>

#include "../Project.h"
#include "../utils/Singleton.h"
#include "IProject.h"
#include "../dbg/Logging.h"

#include "ThreadSafeVariable.h"

#include "ThreadGroup.h"
#include "GameThread.h"
#include <cstdint>

//#define ATLAS_THREAD_YIELD std::this_thread::yield()

namespace Atlas {

	/**
	 * @brief The frame manager is responsible for multi-threading Atlas and managing the game loop
	 * within these threads. It also is flexible in the sense that it will launch a certain number
	 * of threads based on the number of cores.
	 */
	/*class FrameManager : public ThreadGroup {
	private:
		ThreadSafeVariable<IProject*> mUserProject;
		ThreadSafeVariable<volatile bool> mShouldExit;
	public:
		float sFrameTime = 0.0f; // This is provided by the main thread. This is read only and should only be set by the main thread
		volatile bool sIsWaitingForOthers = false; // This is read only and should only be set by the main thread
		volatile bool sExit = false;
		std::unordered_map<std::string, std::shared_ptr<bool>> sThreads; // use this for information about each thread

		volatile bool isWorkerWaiting = false;
		volatile bool isUpdateWaiting = false;

		std::shared_ptr<IProject> userProject;
		std::jthread updateThread;
		std::jthread workThread;

		FrameManager();

		FrameManager(const uint8_t cMaxThreads, ThreadSafeVariable<IProject*> project);

		int RunUpdateThread();

		int RunWorkerThread();

		void launchThreads();

		void killThreads();
	};*/
}