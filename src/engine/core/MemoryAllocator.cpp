/**************************************************************************************************
 * @file MemoryAllocator.cpp
 * 
 * @brief .
 * 
 * @date August 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <iostream>
#include <cstdlib>

#include "MemoryAllocator.h"
#include "Common.h"
#include "MemoryCounter.h"

#if defined(ATLAS_DEBUG) && !defined(ATLAS_NO_ALLOCATION_COUNTER_DEBUG) && !defined(ATLAS_NO_MEMORY_COUNTER) && !defined(ATLAS_ALLOCATION_COUNTER_DEBUG)
	#define ATLAS_ALLOCATION_COUNTER_DEBUG
	
#pragma message(ATLAS_REMINDER "MemoryAllocator.cpp: ATLAS_ALLOCATION_COUNTER_DEBUG is enabled! This is not recommended for production builds as it will severely slow down the engine! Disable this in a release build by defining ATLAS_NO_ALLOCATION_COUNTER_DEBUG in your project's preprocessor definitions.")
#endif // ATLAS_DEBUG && !ATLAS_NO_ALLOCATION_COUNTER_DEBUG && !ATLAS_NO_MEMORY_COUNTER && !ATLAS_ALLOCATION_COUNTER_DEBUG 

#ifndef ATLAS_EXCLUDE_CUSTOM_MEMORY_ALLOCATOR

namespace Atlas {
	AllocFunc Alloc = nullptr;
	ReallocFunc Realloc = nullptr;
	FreeFunc Free = nullptr;

	AlignedAllocFunc AllocAligned = nullptr;
	AlignedFreeFunc FreeAligned = nullptr;

}

void Atlas::RegisterMemoryFunctions(const MemoryFunctionsAggregator& memoryFunctionsAggregator)
{
	Atlas::Alloc = memoryFunctionsAggregator.alloc;
	Atlas::Free = memoryFunctionsAggregator.free;
	Atlas::Realloc = memoryFunctionsAggregator.realloc;

	Atlas::AllocAligned = memoryFunctionsAggregator.allocAligned;
	Atlas::FreeAligned = memoryFunctionsAggregator.freeAligned;

	// TODO: Have this function test the functions to make sure they are valid before exiting the function.

}

void Atlas::RegisterDefaultMemoryFunctions()
{

}

#endif

void* Atlas::MemoryAllocator::allocate(size_t size)
{
#ifdef ATLAS_ALLOCATION_COUNTER_DEBUG
	std::cout << "Attempting to allocate " << size << " bytes of memory..." << std::endl;
#endif // ATLAS_ALLOCATION_COUNTER_DEBUG

	if (size == 0) {
		std::cout << "Cannot allocate 0 bytes of memory!" << std::endl;
		return nullptr;
	}
	else {
		return std::malloc(size);
	}
}

void Atlas::MemoryAllocator::free(void* ptr, size_t size)
{
	if (size == 0) {
		std::cout << "Cannot free 0 bytes of memory!" << std::endl;
		return;
	}
	else {
		std::free(ptr);
	}
}

bool Atlas::MemoryAllocator::isMemoryCapped() const
{
	return false;
}

void* Atlas::MemoryAllocator::alignedAllocate(size_t size, size_t alignment)
{
	return nullptr;
}

void* Atlas::MemoryAllocator::reallocate(void* ptr, size_t oldSize, size_t newSize)
{
	return nullptr;
}

void Atlas::MemoryAllocator::alignedFree(void* ptr, size_t size)
{
}
