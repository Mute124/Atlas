/**************************************************************************************************
 * @file Device.cpp
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <cstdint>
#include <thread>

#include <SDL_cpuinfo.h>

#include "Device.h"
#include "Common.h"





//Atlas::CPUDeviceInfo::CPUDeviceInfo(const int logicalCoreCount, const uint32_t hardwareConcurrency)
//	: mLogicalCoreCount(logicalCoreCount), mHardwareConcurrency(hardwareConcurrency)
//{
//}

Atlas::CPUDeviceInfo::CPUDeviceInfo()
	: mL1CacheLineSize(GetL1CacheLineSize()), mLogicalCoreCount(GetLogicalCoreCount()), mHardwareConcurrency(HardwareConcurrency())
{
}

int Atlas::CPUDeviceInfo::GetL1CacheLineSize()
{
	return SDL_GetCPUCacheLineSize();
}

int Atlas::CPUDeviceInfo::GetLogicalCoreCount()
{
	int coreCount = SDL_GetCPUCount();

	// TODO: Add an exception check

	return coreCount;
}

uint32_t Atlas::CPUDeviceInfo::HardwareConcurrency() {
	return std::thread::hardware_concurrency();
}

int Atlas::CPUDeviceInfo::getL1CacheLineSize() const
{
	return mL1CacheLineSize;
}

int Atlas::CPUDeviceInfo::getLogicalCoreCount() const {
	return mLogicalCoreCount;
}

uint32_t Atlas::CPUDeviceInfo::getHardwareConcurrency() const {
	return mHardwareConcurrency;
}

//void Atlas::CPUDeviceInfo::enumerate()
//{
//}
