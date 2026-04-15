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
#define GLM_ENABLE_EXPERIMENTAL

#include <cstdint>
#include <cmath>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/fwd.hpp>

#define ATLAS_INT64_LIMIT std::numeric_limits<int64_t>::max()
#define ATLAS_INT32_LIMIT std::numeric_limits<int32_t>::max()
#define ATLAS_INT16_LIMIT std::numeric_limits<int16_t>::max()
#define ATLAS_INT8_LIMIT std::numeric_limits<int8_t>::max()

#define ATLAS_UINT64_LIMIT std::numeric_limits<uint64_t>::max()
#define ATLAS_UINT32_LIMIT std::numeric_limits<uint32_t>::max()
#define ATLAS_UINT16_LIMIT std::numeric_limits<uint16_t>::max()
#define ATLAS_UINT8_LIMIT std::numeric_limits<uint8_t>::max()

namespace Atlas {
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using Matrix4 = glm::mat4;

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

	template<typename T_NUMERICAL_TYPE>
	T_NUMERICAL_TYPE Clamp(T_NUMERICAL_TYPE value, T_NUMERICAL_TYPE min, T_NUMERICAL_TYPE max) {		
		// If the value is less than the min, return the min, if the value is greater than the max, return the max
		if (value < min) {
			return min;
		}
		else if (value > max) {
			return max;
		} else {
			return value;
		}
	}
}
