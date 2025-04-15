/**
* @file MemoryAllocator.h
* 
* @brief Provides definitions to handle memory allocation and deallocation within the engine.
* 
* @note This file is not yet implemented.
* 
* @since v0.0.9
*/
#pragma once

#include <cstdint>
#include <memory>

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

	using MemoryAllocationAddress = void*;

	/**
	 * @brief Provides information about the memory usage of the engine.
	 * 
	 * @since v0.0.9
	 */
	struct MemoryAllocationCallInfo {
		size_t memoryAllocationSize;
		MemoryAllocationAddress memoryAllocationAddress;
	};
	

	/**
	 * @brief Provides settings for the memory allocator.
	 * 
	 * @since v0.0.9
	 */
	struct MemoryAllocatorSettings {
		/**
		 * @brief The memory cap of the engine.
		 * 
		 * @since v0.0.9
		 */
		size_t maxMemoryUsage;
	};

	struct MemoryAllocatorStats {
		size_t currentMemoryUsage;
		size_t maxMemoryUsage;
	};

	/**
	 * @brief A base abstract class for memory allocators.
	 * 
	 * @since v0.0.9
	 */
	class AbstractMemoryAllocator {
	private:
		MemoryAllocatorSettings* mMemoryAllocationInfo = nullptr;
		MemoryAllocatorStats* mMemoryAllocatorStats = nullptr;

	protected:
		void setMemoryAllocatorSettings(MemoryAllocatorSettings* memoryAllocationInfo) { mMemoryAllocationInfo = memoryAllocationInfo; }
		MemoryAllocatorSettings* getMemoryAllocatorSettings() const { return mMemoryAllocationInfo; }
		
		void setMemoryAllocatorStats(MemoryAllocatorStats* memoryAllocatorStats) { mMemoryAllocatorStats = memoryAllocatorStats; }
		MemoryAllocatorStats* getMemoryAllocatorStats() const { return mMemoryAllocatorStats; }

	public:
		AbstractMemoryAllocator() {};

		explicit AbstractMemoryAllocator(MemoryAllocatorSettings* memoryAllocationInfo)
			: mMemoryAllocationInfo(memoryAllocationInfo) {
		}

		virtual MemoryAllocationAddress allocate(MemoryAllocationCallInfo* memoryAllocationInfo) = 0;
		virtual void deallocate(MemoryAllocationCallInfo* memoryAllocationInfo) = 0;
	};

	/**
	 * @brief Handles memory allocation and deallocation. This class is also used to help manage the amount of memory used by the engine. This class can also
	 * set the memory limit of the engine (beware that this will cause the engine to crash if the limit is exceeded, so use with caution).
	 * @since v0.0.9
	 */
	class MemoryAllocator : public AbstractMemoryAllocator {
	public:
		MemoryAllocator() = default;

		MemoryAllocator(MemoryAllocatorSettings* memoryAllocationInfo) : AbstractMemoryAllocator(memoryAllocationInfo) {}

		MemoryAllocationAddress allocate(MemoryAllocationCallInfo* memoryAllocationInfo) override {
			MemoryAllocatorStats* stats = getMemoryAllocatorStats();

			if (stats != nullptr && memoryAllocationInfo != nullptr) {

				// copy variables for thread safety

				const size_t currentMemoryUsage = stats->currentMemoryUsage;
				const size_t maxMemoryUsage = stats->maxMemoryUsage;
				const size_t memoryAllocationSize = memoryAllocationInfo->memoryAllocationSize;

				// Check if the allocation can occur without exceeding the memory limit
				if(memoryAllocationInfo->memoryAllocationSize + stats->currentMemoryUsage <= stats->maxMemoryUsage) {
					stats->currentMemoryUsage += memoryAllocationInfo->memoryAllocationSize;

					return malloc(memoryAllocationSize);
				}
			}
			else {
				// return for now.
				return nullptr;
			}
		}

		void deallocate(MemoryAllocationCallInfo* memoryAllocationInfo) override {

			MemoryAllocatorStats* stats = getMemoryAllocatorStats();

			if (stats != nullptr && memoryAllocationInfo != nullptr) {
				delete memoryAllocationInfo->memoryAllocationAddress;

				stats->currentMemoryUsage -= memoryAllocationInfo->memoryAllocationSize;
			}
		}
	};

}