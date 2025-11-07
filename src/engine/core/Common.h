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
#include <cstdint>
#include <string>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <type_traits>

#if defined(ATLAS_ALLOW_ASSERTS) || defined(ATLAS_DEBUG)
	#include <cassert>
#endif

#ifdef __GNUG__
	#include <cxxabi.h>
	#include <memory>
#endif

// Used for some of the math stuff

#include "Core.h"
#include <typeinfo>

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
 * @brief Disables the copy constructor and copy assignment operator for a class defined by the value of @c type.
 * The type that is passed should be the name of the class that should not have the ability to copy itself.
 * In essence, this macro disables the copy constructor and copy assignment operator for the class defined by @c type,
 * thereby making it easier to prevent copying of the class and reduces the amount of code needed to do so.
 * 
 * @since v0.0.1
 */
#define ATLAS_DISALLOW_COPY(type)				\
	type(const type&) = delete;					\
	type& operator=(const type&) = delete;		

#define ATLAS_DISALLOW_MOVE(type)				\
	type(type&&) = delete;						\
	type& operator=(type&&) = delete;

#define ATLAS_DISALLOW_COPY_AND_MOVE(type)		\
	ATLAS_DISALLOW_COPY(type)					\
	ATLAS_DISALLOW_MOVE(type)

#define ATLAS_ALLOW_COPY(type)					\
	type(const type&) = default;				\
	type& operator=(const type&) = default;


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

#ifdef ATLAS_CONDITIONAL_EXPLICIT_SUPPORTED
	#define ATLAS_IMPLICIT explicit(false)
	#define ATLAS_EXPLICIT explicit(true)
#else 
	#define ATLAS_IMPLICIT
	#define ATLAS_EXPLICIT
#endif
//
//#ifdef __cpp_variadic_templates
//	#define ATLAS_VARIADIC(...) __VA_ARGS__
//#endif

namespace Atlas {
	namespace Literals {
		// TODO: add literals
	}

	using std::string;
	using std::cout;
	using std::endl;
	using std::thread;
	using std::this_thread::sleep_for;
	using std::chrono::milliseconds;
	using std::chrono::seconds;
	using std::unordered_map;

	using HighResolutionTimepoint = std::chrono::high_resolution_clock::time_point;

	enum class EResultCode : int32_t {
		Failed = -1,
		Okay = 0,
		NotImplemented,
		InvalidArgument,
	};
	
	template<typename T_NUMERIC_TYPE>
	struct Rectangle {
		T_NUMERIC_TYPE x;
		T_NUMERIC_TYPE y;
		T_NUMERIC_TYPE width;
		T_NUMERIC_TYPE height;
	};

	// Useful for meta-programming
	template<typename T_TYPE>
	struct TypeAudit {
	public:
		enum class ETypeCategory : int8_t {
			Primitive,
			Class,
			Enum,
			Union
		};

	private:
		static inline ETypeCategory GetTypeCategory() noexcept {
			ETypeCategory category{ ETypeCategory::Primitive };
			//|| std::is_interface_v<T_TYPE>
			if (std::is_class_v<T_TYPE>) {
				category = ETypeCategory::Class;
			}
			else if (std::is_enum_v<T_TYPE>) {
				category = ETypeCategory::Enum;
			}
			else if (std::is_union_v<T_TYPE>) {
				category = ETypeCategory::Union;
			}
			// No need to check for primitive type as it is the default value.

			return category;
		}

	public:
		const ETypeCategory typeCategory{ GetTypeCategory() };

		const bool bDefaultConstructible{ std::is_default_constructible_v<T_TYPE> };
		const bool bCopyConstructible{ std::is_copy_constructible_v<T_TYPE> };
		const bool bMoveConstructible{ std::is_move_constructible_v<T_TYPE> };
		
		const bool bCopyAssignable{ std::is_copy_assignable_v<T_TYPE> };
		const bool bMoveAssignable{ std::is_move_assignable_v<T_TYPE> };
		
		const bool bIsPointer{ std::is_pointer_v<T_TYPE> };
		const bool bIsArithmetic{ std::is_arithmetic_v<T_TYPE> };
		const bool bIsIntegral{ std::is_integral_v<T_TYPE> };
		const bool bIsSigned{ std::is_signed_v<T_TYPE> };
		const bool bIsFloatingPoint{ std::is_floating_point_v<T_TYPE> };


		const size_t size{ sizeof(T_TYPE) };
		const size_t alignment{ alignof(T_TYPE) };
	};

	class Validatable {
	private:
		
		bool mbIsValid{ false };
		
	protected:
		bool& getInternalValidityValue() {
			return mbIsValid;
		}
		
		virtual void setValidity(bool bIsValid) {
			mbIsValid = bIsValid;
		}

		virtual void setValid() {
			setValidity(true);
		}

		virtual void setInvalid() {
			setValidity(false);
		}

	public:
		ATLAS_EXPLICIT Validatable(bool bIsValid) : mbIsValid(bIsValid) {}

		Validatable() = default;

		virtual ~Validatable() = default;

		virtual bool isValid() const {
			return mbIsValid;
		}

		ATLAS_IMPLICIT virtual operator bool() const {
			return isValid();
		}
	};

	class Initializable {
	private:

		bool mbIsInit{ false };

	protected:
		bool& getInternalInitValue() {
			return mbIsInit;
		}

		virtual void setInitState(bool bNewInitState) {
			mbIsInit = bNewInitState;
		}

		virtual void setInit() {
			setInitState(true);
		}

		virtual void setNotInit() {
			setInitState(false);
		}

	public:
		ATLAS_EXPLICIT Initializable(bool bIsInit) : mbIsInit(bIsInit) {}

		Initializable() = default;

		virtual ~Initializable() = default;

		virtual bool isInit() const {
			return mbIsInit;
		}

	};

	class InitializableAndValidatable : public Initializable, public Validatable {
	public:
		InitializableAndValidatable(bool bIsInit, bool bIsValid) 
			: Initializable(bIsInit), Validatable(bIsValid) {}

		InitializableAndValidatable() = default;

		virtual bool isValid() const override {
			return Validatable::isValid() && Initializable::isInit();
		}
	};

	template<typename T_WRAPS>
	class HandleWrapperBase : public InitializableAndValidatable {
	private:


		// TODO: find a way to set the default value to either nullptr or 0 depending on the type
		T_WRAPS mHandle;
		const T_WRAPS mInvalidHandle;
	protected:
		T_WRAPS& getInternalHandleValue() {
			return mHandle;
		}

		void setHandle(T_WRAPS handle) {
			if (handle != mInvalidHandle) {
				setInit();
				setValid();
			}
			else {
				setNotInit();
				setInvalid();
			}

			mHandle = handle;
		}
	
	public:

		//using InitializableAndValidatable::InitializableAndValidatable;

		HandleWrapperBase(T_WRAPS handle, T_WRAPS invalidHandle) : InitializableAndValidatable(handle != invalidHandle), mHandle(handle), mInvalidHandle(invalidHandle) {}

		explicit(false) HandleWrapperBase(T_WRAPS handle) : HandleWrapperBase(handle, nullptr) {}

		HandleWrapperBase() = default;

		virtual bool hasValidHandle() const {
			return mHandle != mInvalidHandle;
		}

		virtual bool isValid() const override {
			return InitializableAndValidatable::isValid() && hasValidHandle();
		}

		T_WRAPS getHandle() const {
			return mHandle;
		}

		explicit(false) operator const T_WRAPS& () const {
			return mHandle;
		}

		explicit(false) operator T_WRAPS& () {
			return mHandle;
		}

		//HandleWrapperBase<T_WRAPS>& operator=(const HandleWrapperBase<T_WRAPS>&) = delete; // Delete the default assignment operator

		//HandleWrapperBase<T_WRAPS>& operator=(const HandleWrapperBase<T_WRAPS>& other) noexcept { // Provide a custom move assignment operator
		//	if (this != &other) {
		//		mHandle = other.mHandle;
		//		mInvalidHandle = other.mInvalidHandle;
		//		// ...
		//		// Perform any other necessary operations
		//	}
		//	return *this;
		//}
	};

	class NamedObject {
	private:
		std::string mName{ "None" };

	protected:
		void setName(const std::string& name) {
			mName = name;
		}

	public:
		explicit NamedObject(const std::string& name) : mName(name) {}

		NamedObject() = default;

		std::string getName() const {
			return mName;
		}


	};

	class Counter {
	private:
		int64_t mCount = 0;
		int64_t mMaxCount = -1;
		bool mbIsCapped = false;
	public:
		Counter(int64_t count, int64_t max) : mCount(count), mMaxCount(max), mbIsCapped(max > 0) {}

		ATLAS_EXPLICIT Counter(int64_t count) : Counter(count, -1) {}

		Counter() = default;

		bool isCapped() const {
			return mbIsCapped;
		}

		bool hasReachedMax() const {
			return mbIsCapped && mCount >= mMaxCount;
		}

		void increment(int64_t count) {
			mCount += count;
		}

		void increment() {
			increment(1);
		}

		void decrement(int64_t count) {
			mCount -= count;
		}

		void decrement() {
			decrement(1);
		}

		void reset() {
			mCount = 0;
			mbIsCapped = false;
			mMaxCount = -1;
		}

		int64_t getCount() const {
			return mCount;
		}

		int64_t getMaxCount() const {
			return mMaxCount;
		}

		int64_t getRemainingCount() const {
			return mMaxCount - mCount;
		}

		void setCount(int64_t count) {
			mCount = count;
		}

		void setMaxCount(int64_t max) {
			if (max == -1) {
				mMaxCount = -1;
				mbIsCapped = false;
			} else {
				mMaxCount = max;
				mbIsCapped = true;
			}
		}

		void setCountAndMaxCount(int64_t count, int64_t max) {
			setCount(count);
			setMaxCount(max);
		}
	};

	template<typename T_BASE>
	class Singleton {
	protected:
		//friend class T_BASE;
	
	public:

		// Delete copy constructor and assignment operator to prevent copying
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		// Static method to get the single instance
		static T_BASE& getInstance() {
			static T_BASE instance(); // Local static instance, initialized once
			return instance;
		}

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

	//template<typename T>
	//inline bool HasValue(T valueToCheck, TypeAudit<T> const& typeAudit = TypeAudit<T>{}) {
	//	
	//}
}
