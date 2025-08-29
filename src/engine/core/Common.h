/**************************************************************************************************
 * @file Common.h
 * 
 * @brief Common includes and definitions that are used throughout the engine. There are going to
 * be very limited amounts of actual engine includes, so this is not a replacement for that.
 * Instead, this is a way to centralize the includes and definitions that are used throughout the
 * engine and avoid include-hell
 * 
 * @details This file has a few configurations:
 * - ATLAS_ALLOW_ASSERTS
 * - ATLAS_DEBUG
 * If ATLAS_ALLOW_ASSERTS or ATLAS_DEBUG is defined, Atlas will turn on assertions. If you do not
 * want Atlas to do assertions, make sure you undefine ATLAS_ALLOW_ASSERTS. 
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <string>
#include <thread>
#include <any>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <chrono>
#include <type_traits>

#if defined(ATLAS_ALLOW_ASSERTS) || defined(ATLAS_DEBUG)
	#include <cassert>
#endif

// Used for some of the math stuff
#include <glm/glm.hpp>

#include "Core.h"

#if defined(ATLAS_ALLOW_ASSERTS) || defined(ATLAS_DEBUG)
	/**
	* @brief If enabled (see the warning in the documentation), this will assert the specified condition
	* by using the C++ assert function.
	* 
	* @warning This macro function will only do anything if it is enabled (see this file's
	* documentation or the see also section for how it is enabled).
	* 
	* @note The reason for why the assertions are wrapped is to give developers the ability to
	* actually prevent Atlas from doing assertions
	* 
	* @param condition An expression that @b MUST result in some kind of boolean value.
	* 
	* @param message The message to display if the assertion fails.
	* 
	* @since v0.0.1
	*/
	#define ATLAS_ASSERT(condition, message) assert(condition && message)

	/**
	* @brief If enabled (see the warning in the documentation), this will do a static assertion for 
	* the specified condition by using the C++ static_assert function.
	*
	* @warning This macro function will only do anything if it is enabled (see this file's
	* documentation or the see also section for how it is enabled).
	*
	* @note The reason for why the assertions are wrapped is to give developers the ability to
	* actually prevent Atlas from doing assertions
	*
	* @param condition An expression that @b MUST result in some kind of boolean value.
	*
	* @param message The message to display if the assertion fails.
	*
	* @since v0.0.1
	*/
	#define ATLAS_STATIC_ASSERT(condition, message) static_assert(condition && message)

#else 
	/**
	* @brief If enabled (see the warning in the documentation), this will assert the specified condition 
	* by using the C++ assert function.
	*
	* @warning This macro function will only do anything if it is enabled (see this file's
	* documentation or the see also section for how it is enabled).
	* 
	* @note The reason for why the assertions are wrapped is to give developers the ability to
	* actually prevent Atlas from doing assertions
	* 
	* @param condition An expression that @b MUST result in some kind of boolean value.
	*
	* @param message The message to display if the assertion fails.
	*
	* @since v0.0.1
	*/
	#define ATLAS_ASSERT(condition, message) 
	
	/**
	* @brief If enabled (see the warning in the documentation), this will do a static assertion for
	* the specified condition by using the C++ static_assert function.
	*
	* @warning This macro function will only do anything if it is enabled (see this file's
	* documentation or the see also section for how it is enabled).
	*
	* @note The reason for why the assertions are wrapped is to give developers the ability to
	* actually prevent Atlas from doing assertions
	*
	* @param condition An expression that @b MUST result in some kind of boolean value.
	*
	* @param message The message to display if the assertion fails.
	*
	* @since v0.0.1
	*/
	#define ATLAS_STATIC_ASSERT(condition, message) 

#endif // ATLAS_ALLOW_ASSERTS || ATLAS_DEBUG

#define ATLAS_TYPE_NAME(...) __VA_ARGS__

#define ATLAS_STRINGIFY(...) #__VA_ARGS__


/**
* @brief Since the location of the shared libraries and executables can vary, this is a simple solution to this.
* During Distribution builds, the path is more exact, however the rest are not because these
* will be in the build directory rather than the final distribution directory.
* @since v0.0.1
*/
#define ATLAS_TOP_LAYER_DIR static_cast<std::string>(ATLAS_TOP_LAYER)
#define ATLAS_ASSET_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/game/assets/");
#define ATLAS_DATA_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/game/data/");
#define ATLAS_GAME_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/game/");
#define ATLAS_TEMP_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/temp/");

#define ATLAS_NULL_PNG ATLAS_ASSET_DIR + static_cast<std::string>("/engine/null.png")
#define ATLAS_NULL_JPG ATLAS_ASSET_DIR + static_cast<std::string>("/engine/null.jpg")
#define ATLAS_NULL_JPEG static_cast<std::string>(ATLAS_ASSET_DIR) + static_cast<std::string>("/engine/null.jpeg")
#define ATLAS_NULL_GIF ATLAS_ASSET_DIR + static_cast<std::string>("/engine/null.gif"

namespace Atlas {

	using std::string;
	using std::cout;
	using std::endl;
	using std::thread;
	using std::this_thread::sleep_for;
	using std::chrono::milliseconds;
	using std::chrono::seconds;
	using std::unordered_map;

	
	template<typename T_NUMERIC_TYPE>
	struct Rectangle {
		T_NUMERIC_TYPE x;
		T_NUMERIC_TYPE y;
		T_NUMERIC_TYPE width;
		T_NUMERIC_TYPE height;
	};



	/**
	 * @brief An interface for classes that want to be printable.
	 * 
	 * @remarks This is useful for debugging and logging as it provides a way to get a string representation of the object.
	 * 
	 * @note It is not recommended that you use this interface on everything as it may cause performance issues.
	 */
	class IStringifiedObject {
	public:
		IStringifiedObject() = default;

		virtual std::string toString() const = 0;

		virtual operator std::string() const = 0;
	};

	/**
	 * @brief An "inherit-and-forget" class that implements the IPrettyObject interface for you. This class uses a data-driven approach to get the string representation of the object.
	 * 
	 * @tparam T_CHILD The class that has inherited from this.
	 */
	template<typename T_CHILD>
	class StringifiedObject : public IStringifiedObject {
	public:

		StringifiedObject() = default;

		virtual std::string toString() const override {
			return ATLAS_TYPE_NAME(typeid(T_CHILD).name());
		}

		virtual operator std::string() const override {
			return toString();
		}
	};

	template<typename T>
	class ReferenceHolder {
	private:
		T& mValue;
	public:

		explicit ReferenceHolder(T& value) : mValue(value) {}

		void set(T& value) {
			mValue = value;
		}

		T& get() {
			return mValue;
		}

	};

	template<typename T>
	inline T AlignUp(T value, uint64_t alignment) {
		return T((uint64_t(value) + alignment - 1) & ~(alignment - 1));
	}
}
