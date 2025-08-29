#include <cstdint>

#include "GameThreader.h"

void Atlas::GameThreader::setMaxThreadCount(const uint8_t threadCount)
{
	mMaxThreadCount = threadCount;
}

uint8_t Atlas::GameThreader::getMaxThreadCount()
{
	return this->mMaxThreadCount;
}

void Atlas::GameThreader::addScheduler(std::string const& schedulerName, uint8_t numThreads)
{
	if (this->mThreadSchedulerMap.contains(schedulerName)) {
		return;
	}
	// Add some safety checks here, because this is not very safe at all.

	

	this->mThreadSchedulerMap[schedulerName] = std::make_unique<Atlas::GameThreadScheduler>(numThreads);
	this->mThreadSchedulerMap[schedulerName]->init(numThreads);

	this->mThreadCount += numThreads;
}

void Atlas::GameThreader::removeScheduler(std::string const& schedulerName)
{
	this->mThreadCount -= this->mThreadSchedulerMap[schedulerName]->getThreadCount();

	this->mThreadSchedulerMap.erase(schedulerName);

}

void Atlas::GameThreader::init(uint8_t threadCount)
{
}

void Atlas::GameThreader::init()
{
}

void Atlas::GameThreader::cleanup()
{
}

std::unique_ptr<Atlas::GameThreadScheduler>& Atlas::GameThreader::getScheduler(std::string const& schedulerName) {
	return this->mThreadSchedulerMap.at(schedulerName);
}
