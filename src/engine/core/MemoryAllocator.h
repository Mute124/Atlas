/**
* @file MemoryAllocator.h
* 
* @brief Provides definitions to handle memory allocation and deallocation within the engine.
* 
* @note The code is similar to Jolt Physics' memory allocator code. (Thank you Jolt Physics!)
* 
* @note This file is not yet implemented.
* 
* @since v0.0.9
*/
#pragma once

#include <cstdint>
#include <memory>

#include "MakeNonCopyable.h"
#include "MemoryCounter.h"
#include "Common.h"

/**
* @brief Converts kilobytes to bytes.
* 
* @param kb The kilobytes to convert.
* 
* @return The number of bytes.
* 
* @since v0.0.9
*/
#define ATLAS_CONVERT_KB_TO_BYTES(kb) (kb * 1024)

/**
* @brief Converts megabytes to bytes.
* 
* @param mb The megabytes to convert.
* 
* @return The number of bytes.
* 
* @since v0.0.9
*/
#define ATLAS_CONVERT_MB_TO_BYTES(mb) (mb * 1024 * 1024)

/**
* @brief Converts gigabytes to bytes.
* 
* @param gb The gigabytes to convert.
* 
* @return The number of bytes.
*/
#define ATLAS_CONVERT_GB_TO_BYTES(gb) (gb * 1024 * 1024 * 1024)

/**
* @brief The minimum amount of memory that the engine can use in bytes.
* 
* @remarks This is used to ensure that the engine has enough memory to run.
* 
* @warning The engine will crash if the memory limit is exceeded.
* 
* @note This value is set to 100MB by default.
* 
* @since v0.0.9
*/
#define ATLAS_MINIMUM_MEMORY_USAGE ATLAS_CONVERT_MB_TO_BYTES(100)

namespace Atlas {

#ifndef ATLAS_EXCLUDE_CUSTOM_MEMORY_ALLOCATOR
	// Names of functions are shortened for ease of use

	using AllocFunc = void*(*)(size_t size);
	using ReallocFunc = void* (*)(void* blockPtr, size_t oldSize, size_t newSize);
	using FreeFunc = void(*)(void* blockPtr);

	using AlignedAllocFunc = void*(*)(size_t size, size_t alignment);
	using AlignedFreeFunc = void(*)(void* blockPtr);

	struct MemoryFunctionsAggregator {
		// The user can set these functions or use the default (depending on what function they call during initialization)
		
		AllocFunc alloc;
		ReallocFunc realloc;
		FreeFunc free;

		AlignedAllocFunc allocAligned;
		AlignedFreeFunc freeAligned;
	};

	extern AllocFunc Alloc;
	extern ReallocFunc Realloc;
	extern FreeFunc Free;

	extern AlignedAllocFunc AllocAligned;
	extern AlignedFreeFunc FreeAligned;

	// Explicitly defining the namespace here just in case another library defines a function with the same name
	#define ATLAS_INTERNAL_ALLOC(size) Atlas::Alloc(size)
	#define ATLAS_INTERNAL_FREE(ptr) Atlas::Free(ptr)
	// This should not be const 
	//static MemoryFunctionsAggregator GlobalMemoryFunctionsAggregator;



	#define ATLAS_OVERRIDE_MEMORY_ALLOCATOR \
		inline void *operator new (size_t count)											{ return ATLAS_INTERNAL_ALLOC(count); } \
		inline void operator delete (void *ptr) noexcept									{ ATLAS_INTERNAL_FREE(ptr); } \
		inline void *operator new[] (size_t count)											{ return ATLAS_INTERNAL_ALLOC(count); } \
		inline void operator delete[] (void *ptr) noexcept								{ ATLAS_INTERNAL_FREE(ptr); } \
		inline void *operator new (size_t count, std::align_val_t alignment)				{ return Atlas::AllocAligned(count, static_cast<size_t>(alignment)); } \
		inline void operator delete (void *ptr, [[maybe_unused]] std::align_val_t alignment) noexcept	{ Atlas::FreeAligned(ptr); } \
		inline void *operator new[] (size_t count, std::align_val_t alignment)				{ return Atlas::AllocAligned(count, static_cast<size_t>(alignment)); } \
		inline void operator delete[] (void *ptr, [[maybe_unused]] std::align_val_t alignment) noexcept	{ Atlas::FreeAligned(ptr); } \
		inline void *operator new ([[maybe_unused]] size_t count, void *ptr) noexcept	{ return ptr; } \
		inline void operator delete ([[maybe_unused]] void *ptr, [[maybe_unused]] void *place) noexcept { /* Do nothing */ } \
		inline void *operator new[] ([[maybe_unused]] size_t count, void *ptr) noexcept	{ return ptr; } \
		inline void operator delete[] ([[maybe_unused]] void *ptr, [[maybe_unused]] void *place) noexcept { /* Do nothing */ }
		

	void RegisterMemoryFunctions(const MemoryFunctionsAggregator &memoryFunctionsAggregator);
	void RegisterDefaultMemoryFunctions();
#else
	#error "Please dont define ATLAS_EXCLUDE_CUSTOM_MEMORY_ALLOCATOR as Atlas has not yet implemented the option you chose."
#endif

	//using MemoryAllocationAddress = void*;

	///**
	// * @brief Provides information about the memory usage of the engine.
	// * 
	// * @since v0.0.9
	// */
	//struct MemoryAllocationCallInfo {
	//	size_t memoryAllocationSize;
	//	MemoryAllocationAddress memoryAllocationAddress;
	//};
	//

	///**
	// * @brief Provides settings for the memory allocator.
	// * 
	// * @since v0.0.9
	// */
	//struct MemoryAllocatorSettings {
	//	/**
	//	 * @brief The memory cap of the engine.
	//	 * 
	//	 * @since v0.0.9
	//	 */
	//	size_t maxMemoryUsage;
	//};

	//struct MemoryAllocatorStats {
	//	size_t currentMemoryUsage;
	//	size_t maxMemoryUsage;
	//};

	///**
	// * @brief A base abstract class for memory allocators.
	// * 
	// * @since v0.0.9
	// */
	//class AbstractMemoryAllocator {
	//private:
	//	MemoryAllocatorSettings* mMemoryAllocationInfo = nullptr;
	//	MemoryAllocatorStats* mMemoryAllocatorStats = nullptr;

	//protected:
	//	void setMemoryAllocatorSettings(MemoryAllocatorSettings* memoryAllocationInfo) { mMemoryAllocationInfo = memoryAllocationInfo; }
	//	MemoryAllocatorSettings* getMemoryAllocatorSettings() const { return mMemoryAllocationInfo; }
	//	
	//	void setMemoryAllocatorStats(MemoryAllocatorStats* memoryAllocatorStats) { mMemoryAllocatorStats = memoryAllocatorStats; }
	//	MemoryAllocatorStats* getMemoryAllocatorStats() const { return mMemoryAllocatorStats; }

	//public:
	//	AbstractMemoryAllocator() {

	//	};

	//	explicit AbstractMemoryAllocator(MemoryAllocatorSettings* memoryAllocationInfo)
	//		: mMemoryAllocationInfo(memoryAllocationInfo) {
	//	}

	//	virtual MemoryAllocationAddress allocate(size_t memoryAllocationInfo) = 0;
	//	virtual void deallocate(MemoryAllocationCallInfo* memoryAllocationInfo) = 0;
	//};

	///**
	// * @brief Handles memory allocation and deallocation. This class is also used to help manage the amount of memory used by the engine. This class can also
	// * set the memory limit of the engine (beware that this will cause the engine to crash if the limit is exceeded, so use with caution).
	// * @since v0.0.9
	// */
	//class MemoryAllocator : public AbstractMemoryAllocator {
	//public:
	//	MemoryAllocator() = default;

	//	MemoryAllocator(MemoryAllocatorSettings* memoryAllocationInfo) : AbstractMemoryAllocator(memoryAllocationInfo) {}

	//	MemoryAllocationAddress allocate(size_t memoryAllocationInfo) override {
	//		//MemoryAllocatorStats* stats = getMemoryAllocatorStats();

	//		//if (stats != nullptr && memoryAllocationInfo != nullptr) {

	//		//	// copy variables for thread safety

	//		//	const size_t currentMemoryUsage = stats->currentMemoryUsage;
	//		//	const size_t maxMemoryUsage = stats->maxMemoryUsage;
	//		//	const size_t memoryAllocationSize = memoryAllocationInfo->memoryAllocationSize;

	//		//	// Check if the allocation can occur without exceeding the memory limit
	//		//	if(memoryAllocationInfo->memoryAllocationSize + stats->currentMemoryUsage <= stats->maxMemoryUsage) {
	//		//		stats->currentMemoryUsage += memoryAllocationInfo->memoryAllocationSize;

	//		//		return malloc(memoryAllocationSize);
	//		//	}
	//		//}
	//		//else {
	//		//	// return for now.
	//		//	return nullptr;
	//		//}
	//	}

	//	void deallocate(MemoryAllocationCallInfo* memoryAllocationInfo) override {

	//		//MemoryAllocatorStats* stats = getMemoryAllocatorStats();

	//		//if (stats != nullptr && memoryAllocationInfo != nullptr) {
	//		//	delete memoryAllocationInfo->memoryAllocationAddress;

	//		//	stats->currentMemoryUsage -= memoryAllocationInfo->memoryAllocationSize;
	//		//}
	//	}
	//};

	// The below objects are NOT currently used and are more of a potential feature for the future.

	struct MemoryBlock {
		uint8_t* startAddress = nullptr;
		size_t topAddress = 0;
		size_t blockSize = 0;

		// Alignment in bytes
		size_t alignment = 0;
	};

	class IMemoryAllocator : public MakeNonCopyable {
	public:
		// May neede to be unsigned int. This is something that should be tested.
		virtual void* allocate(size_t size) = 0;

		virtual void* alignedAllocate(size_t size, size_t alignment) = 0;

		virtual void* reallocate(void* ptr, size_t oldSize, size_t newSize) = 0;

		virtual void free(void* ptr, size_t size) = 0;

		virtual void alignedFree(void* ptr, size_t size) = 0;

		virtual bool isMemoryCapped() const = 0;
	};

	class MemoryAllocator : public IMemoryAllocator {
	private:
		MemoryBlock mMemoryBlock;
	
	public:

		virtual void* allocate(size_t size) override;

		virtual void* alignedAllocate(size_t size, size_t alignment) override;

		virtual void* reallocate(void* ptr, size_t oldSize, size_t newSize) override;

		virtual void free(void* ptr, size_t size) override;

		virtual void alignedFree(void* ptr, size_t size) override;
		
		virtual bool isMemoryCapped() const override;
	};
}