#pragma once
#include <unordered_map>
#include <any>
#include <functional>

#include "ThreadSafeVariable.h"

namespace Atlas {

	/**
	 * @brief Handles memory allocation and deallocation. This class is also used to help manage the amount of memory used by the engine. This class can also
	 * set the memory limit of the engine (beware that this will cause the engine to crash if the limit is exceeded, so use with caution).
	 * @since v0.0.9
	 */
	template<typename T_POINTER, size_t T_MEMORY_LIMIT>
	class MemoryAllocator {
	private:
		ThreadSafeVariable<size_t> mMemoryUsed = 0;
		const size_t mMemoryLimit = T_MEMORY_LIMIT;

		template<typename ...T_CONSTRUCTOR_ARGS>
		T_POINTER* construct(T_CONSTRUCTOR_ARGS... args) {
			return new T_POINTER(args...);
		}

		void setMemoryUsed(size_t size) {
			mMemoryUsed.set(size);
		}
	public:
		
		
		struct AllocationResult {
			T_POINTER* ptr = nullptr;
			bool success = false;
		};

		template<typename ...T_CONSTRUCTOR_ARGS>
		AllocationResult allocate(size_t size, std::function<T_POINTER*(...T_CONSTRUCTOR_ARGS)> constructionFunction, T_CONSTRUCTOR_ARGS... args) {
			AllocationResult result = AllocationResult();

			if (mMemoryUsed.get() + size > mMemoryLimit) {
				// return it immediately since it can't be allocated
				return result;
			}

			result.ptr = constructionFunction(args...);
			result.success = true;

			mMemoryUsed.set(mMemoryUsed.get() + size);

			return result;
		}

		template<typename ...T_CONSTRUCTOR_ARGS>
		AllocationResult allocate(size_t size, T_CONSTRUCTOR_ARGS... args) {
			return this->allocate<T_POINTER>(size, construct, args...);
		}

		template<typename ...T_CONSTRUCTOR_ARGS>
		AllocationResult allocate(T_CONSTRUCTOR_ARGS... args) {
			return this->allocate<T_POINTER>(sizeof(T_POINTER), args...);
		}

		AllocationResult allocate(size_t size) {
			return this->allocate<void>(size, void);
		}

		void deallocate(T_POINTER* ptr) {
			delete ptr;
			setMemoryUsed(mMemoryUsed.get() - sizeof(T_POINTER));
		}
	};

	template<class T_MEMORY_ALLOCATOR>
	class MemoryManager {
	private:
		template<typename T_VARIABLE_TYPE>
		class MemoryBlock final {
		private:
			T_VARIABLE_TYPE mVariable;
			const size_t mSize;
		public:

			MemoryBlock(T_VARIABLE_TYPE variable, size_t size) : mVariable(variable), mSize(size) {}

			MemoryBlock(T_VARIABLE_TYPE variable) : mVariable(variable), mSize(sizeof(T_VARIABLE_TYPE)) {}

			MemoryBlock() : mVariable(T_VARIABLE_TYPE()), mSize(sizeof(T_VARIABLE_TYPE)) {}

			size_t getSize() const {
				return mSize;
			}

		};

	public:
	};

}