/**************************************************************************************************
 * @file Math.h
 * 
 * @brief A general collection of math functions that can be used by the engine or any other project.
 * 
 * @date September 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <cstdint>
#include <cmath>
#include <type_traits>

namespace Atlas {
	
	template<typename T_INPUT>
	concept Numerical = std::is_arithmetic_v<T_INPUT>;

	/**
	 * @brief Normalizes a given value to be between a min and max value.
	 * 
	 * @param value The value to normalize
	 * @param min The minimum value that can be returned
	 * @param max The maximum value that can be returned
	 * 
	 * @tparam T_NUMERICAL_TYPE The numerical type to use. This can be any numerical type
	 * 
	 * @return A normalized value of the given value
	 * 
	 * @since v0.0.1
	 */
	template<Numerical T_NUMERICAL_TYPE>
	T_NUMERICAL_TYPE Normalize(T_NUMERICAL_TYPE value, T_NUMERICAL_TYPE min, T_NUMERICAL_TYPE max) {
		return (value - min) / (max - min);
	}
}
