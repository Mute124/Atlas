#pragma once

#include <cstdint>
#include <bitset>

namespace Atlas {

	template<typename T_BITSET_SIZE>
	class ObjectOptions {
	public:
		std::bitset<T_BITSET_SIZE> mOptions;

		ObjectOptions() = default;
		
		void setOption(size_t option, bool bValue) {
			mOptions.set(option, bValue);
		}

		void toggleOption(size_t option) {
			mOptions.flip(option);
		}

		void resetOption(size_t option) {
			mOptions.reset(option);
		}

		bool isOptionSet(size_t option) const {
			return mOptions.test(option);
		}

		bool areAllOptionsSet() const {
			return mOptions.all();
		}

		bool areAnyOptionsSet() const {
			return mOptions.any();
		}

		bool areNoOptionsSet() const {
			return mOptions.none();
		}
	};
}
