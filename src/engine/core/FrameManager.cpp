#include "FrameManager.h"
#include "IProject.h"
#include "ThreadSafeVariable.h"
#include "ThreadGroup.h"
#include "../dbg/Logging.h"
#include <chrono>
#include <cstdint>
#include <functional>
#include <thread>
/*
Atlas::FrameManager::FrameManager() : ThreadGroup() {}

Atlas::FrameManager::FrameManager(const uint8_t cMaxThreads, ThreadSafeVariable<IProject*> project) 
	: ThreadGroup(), mUserProject(project) {
	sIsWaitingForOthers = true;
	addThread("UpdateThread", std::bind(&FrameManager::RunUpdateThread, this));
	addThread("WorkerThread", std::bind(&FrameManager::RunWorkerThread, this));

	while (sIsWaitingForOthers && !sExit) {
		if (isWorkerWaiting && isUpdateWaiting) {
			Log("All threads are ready");
			this->mUserProject.get()->postInit();

			//this->userProject->postInit();
			sIsWaitingForOthers = false;
			break;
		}
		else {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}
	}

	userProject->run(0, nullptr);
}

int Atlas::FrameManager::RunUpdateThread()
{
	isUpdateWaiting = true;

	while (sIsWaitingForOthers && !sExit) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	while (!sExit) {

		userProject->update();

		userProject->preObjectUpdate();
		userProject->objectUpdate();
		userProject->postObjectUpdate();

		userProject->prePhysicsUpdate();
		userProject->physicsUpdate();
		userProject->postPhysicsUpdate();

		// This is here because it reduces CPU consumption
		ATLAS_THREAD_YIELD;

		// make it so this runs at 60 FPS
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	return 0;
}

int Atlas::FrameManager::RunWorkerThread()
{
	this->userProject->preInit();
	this->userProject->init(0, nullptr);

	isWorkerWaiting = true;
	while (sIsWaitingForOthers && !sExit) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	while (!sExit) {
		this->userProject->workingUpdate();

		// This is here because it reduces CPU consumption
		ATLAS_THREAD_YIELD;
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 0;
}

void Atlas::FrameManager::launchThreads()
{
	sIsWaitingForOthers = true;
	updateThread = std::jthread([&]() {
		RunUpdateThread();
		});

	updateThread.detach();

	workThread = std::jthread([&]() {

		RunWorkerThread();
		});

	workThread.detach();
	while (sIsWaitingForOthers && !sExit) {
		if (isWorkerWaiting && isUpdateWaiting) {
			Log("All threads are ready");
			this->userProject->postInit();
			sIsWaitingForOthers = false;
			break;
		}
		else {

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}
	}

	userProject->run(0, nullptr);
}

void Atlas::FrameManager::killThreads() {
	sExit = true;
	userProject->cleanup(0);

	updateThread.join();
	workThread.join();
}*/
