#include "MemoryCounter.h"

Atlas::MemoryCounter::MemoryCounter(const size_t cMaxMemoryUsageInBytes)
	: mMemoryCap(cMaxMemoryUsageInBytes), mCurrentMemoryUsageInBytes(0) {
}

size_t Atlas::MemoryCounter::getMaxMemoryUsage() const {
	return mMemoryCap;
}

size_t Atlas::MemoryCounter::getCurrentMemoryUsage() const {
	return mCurrentMemoryUsageInBytes;
}

bool Atlas::MemoryCounter::isMemoryUsageExceeded() const {
	return mCurrentMemoryUsageInBytes > getMaxMemoryUsage();
}

bool Atlas::MemoryCounter::isMemoryUsageWithinBounds() const {
	// When memory usage is less than or equal to max memory usage, return true
	return mCurrentMemoryUsageInBytes <= mMemoryCap;
}

bool Atlas::MemoryCounter::willMemoryUsageExceed(const size_t cIncrement) const {
	return mCurrentMemoryUsageInBytes + cIncrement > mMemoryCap;
}

void Atlas::MemoryCounter::setCurrentMemoryUsage(const size_t cCurrentMemoryUsageInBytes) {
	mCurrentMemoryUsageInBytes = cCurrentMemoryUsageInBytes;
}

void Atlas::MemoryCounter::incrementCurrentMemoryUsage(const size_t cIncrement) {
	if (willMemoryUsageExceed(cIncrement)) {
		return;
	}

	mCurrentMemoryUsageInBytes += cIncrement;
}

void Atlas::MemoryCounter::decrementCurrentMemoryUsage(const size_t cDecrement) {
	mCurrentMemoryUsageInBytes -= cDecrement;
}

void Atlas::MemoryCounter::reset()
{
	setCurrentMemoryUsage(0);
}
