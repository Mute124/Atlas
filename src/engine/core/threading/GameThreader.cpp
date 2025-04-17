#include <cstdint>

#include "GameThreader.h"


void Atlas::GameThreader::addScheduler(std::string const& schedulerName, uint8_t numThreads)
{
	if (this->mThreadSchedulerMap.contains(schedulerName)) {
		return;
	}

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
