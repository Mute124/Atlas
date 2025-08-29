#pragma once

namespace Atlas {
	class MemoryCounter {
	private:
		const size_t mMemoryCap;
		
		size_t mCurrentMemoryUsageInBytes;
	public:

		explicit MemoryCounter(const size_t cMaxMemoryUsageInBytes);

		size_t getMaxMemoryUsage() const;

		size_t getCurrentMemoryUsage() const;

		bool isMemoryUsageExceeded() const;

		bool isMemoryUsageWithinBounds() const;

		bool willMemoryUsageExceed(const size_t cIncrement) const;

		void setCurrentMemoryUsage(const size_t cCurrentMemoryUsageInBytes);

		void incrementCurrentMemoryUsage(const size_t cIncrement);

		void decrementCurrentMemoryUsage(const size_t cDecrement);

		void reset();

		// operators
		
		MemoryCounter& operator=(const MemoryCounter& other) {
			mCurrentMemoryUsageInBytes = other.mCurrentMemoryUsageInBytes;
			return *this;
		}

		// increment
		MemoryCounter& operator++() {
			incrementCurrentMemoryUsage(1);
			return *this;
		}

		MemoryCounter operator++(int) {
			MemoryCounter temp = *this;
			incrementCurrentMemoryUsage(1);
			return temp;
		}

		// decrement
		MemoryCounter& operator--() {
			decrementCurrentMemoryUsage(1);
			return *this;
		}

		MemoryCounter operator--(int) {
			MemoryCounter temp = *this;
			decrementCurrentMemoryUsage(1);
			return temp;
		}

		size_t operator+=(const size_t cIncrement) {
			incrementCurrentMemoryUsage(cIncrement);
			return mCurrentMemoryUsageInBytes;
		}
	};
}
