#include "GameThread.h"

#ifndef ATLAS_ENABLE_THREADING

void Atlas::GameThread::setIdleState(bool newState) {
	this->mIsIdle.store(newState);
}

void Atlas::GameThread::setIsReady(bool newReadinessState) {
	this->mIsReady.store(newReadinessState);
}

Atlas::GameThread::GameThread(ThreadLaunchInfo const& launchInfo) : mThreadFunction(launchInfo.threadFunction) {

}

bool Atlas::GameThread::isIdle() {
	return this->mIsIdle;
}

bool Atlas::GameThread::isDetached()
{
	return false;
}

bool Atlas::GameThread::isReady() {
	return this->mIsReady;
}

bool Atlas::GameThread::isJoinable()
{
	return this->mThread.joinable();
}

void Atlas::GameThread::detach() {
	this->mThread.detach();
}

void Atlas::GameThread::join() {
	this->mThread.join();
}

void Atlas::GameThread::execute() {
	this->mThread = std::jthread(this->mThreadFunction);
}

#endif