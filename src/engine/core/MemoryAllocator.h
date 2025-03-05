#pragma once

namespace Atlas {

	/**
	 * @brief Handles memory allocation and deallocation. This class is also used to help manage the amount of memory used by the engine. This class can also
	 * set the memory limit of the engine (beware that this will cause the engine to crash if the limit is exceeded, so use with caution).
	 * @since v0.0.9
	 */
	class MemoryAllocator {
	public:
		static void* allocate(size_t size);
		static void deallocate(void* ptr);
	};

}