/**************************************************************************************************
 * @file ThreadSafeVariable.h
 * 
 * @brief 
 * 
 * @date April 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <type_traits>
#include <atomic>
#include <stdexcept>
#include "../Common.h"

namespace Atlas {
	/**
	 * @brief This class wraps a variable (specified by T_VARIABLE_TYPE) and makes
	 * it thread-safe. 
	 * 
	 * @details When ThreadSafeVariable::execute() is called, it will lock 
	 * this variable and execute the function that is passed to it. Furthermore,
	 * it will also lock when the set() function, is called.
	 * 
	 * @warning The variable that is wrapped by this class must have a default
	 * constructor. If the variable does not have a default constructor, then
	 * this class will throw an exception (@ref MissingDefaultConstructorException).
	 * 
	 * @tparam T_VARIABLE_TYPE The type of variable to wrap. This is used as a variable in 
	 * mValue.
	 * 
	 * @tparam T_MUTEX What type of mutex to use. This defaults to std::mutex
	 * 
	 * @tparam T_LOCK_GUARD What type of lock_guard to use. This defaults to std::lock_guard<T_MUTEX>
	 * 
	 * @since v0.0.9
	 */
	template <typename T_VARIABLE_TYPE, typename T_MUTEX = std::shared_mutex>
	class ThreadSafeVariable {
	private:

		// Static so that it is only calculated once per type
		const static inline TypeAudit<T_VARIABLE_TYPE> sValueTypeAudit{};


		/**
		 * @brief This is the variable that is wrapped by this class.
		 *
		 * @since v0.0.9
		 */
		T_VARIABLE_TYPE mValue;
		mutable T_MUTEX mMutex;

		std::atomic<bool> mValueSet{ false };

	public:

		ThreadSafeVariable() = default;

		explicit ThreadSafeVariable(const T_VARIABLE_TYPE& value) : mValue(value), mValueSet(true) {}
		explicit ThreadSafeVariable(T_VARIABLE_TYPE&& value) : mValue(std::move(value)), mValueSet(true) {}

		ThreadSafeVariable(const ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX>&) = delete;
		ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX>& operator=(const ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX>&) = delete;

		// Public because there is no reason to hide this function 
		bool canDefaultConstructType() const noexcept { 
			return sValueTypeAudit.bDefaultConstructible;
		}

		bool isPointerType() const noexcept { 
			return sValueTypeAudit.bIsPointer;
		}

		template <typename Func>
		auto execute(Func&& func) -> decltype(func(std::declval<T_VARIABLE_TYPE&>())) {
			std::unique_lock<T_MUTEX> lock(mMutex);
			return func(mValue);
		}

		T_VARIABLE_TYPE getCopy() const {
			std::shared_lock<T_MUTEX> lock(mMutex);
			return mValue;
		}

		const T_VARIABLE_TYPE& get() const {
			std::shared_lock<T_MUTEX> lock(mMutex);
			return mValue;
		}

		void set(const T_VARIABLE_TYPE& newValue) {
			std::unique_lock<T_MUTEX> lock(mMutex);
			mValue = newValue;

			// Update the value set flag
			mValueSet.store(true);
		}
		
		void set(T_VARIABLE_TYPE&& newValue) {
			set(std::move(newValue));
		}

		//void reset() {
		//	std::unique_lock<T_MUTEX> lock(mMutex);

		//	if (canDefaultConstructType()) {


		//		mValue = T_VARIABLE_TYPE();
		//	} else if (isPointerType()) {
		//		mValue = nullptr;
		//	} else {
		//		throw std::runtime_error("ThreadSafeVariable::reset() - The variable that is wrapped by this class must have a default constructor.");
		//	}
		//}

		bool operator==(const ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX>& other) const {
			return mValue == other.mValue;
		}

		bool operator==(const T_VARIABLE_TYPE& other) const {
			return mValue == other;
		}

		// set operator
		ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX>& operator=(const T_VARIABLE_TYPE& other) {
			set(other);
			return *this;
		}

	};

	template<typename T_VARIABLE_TYPE, typename T_MUTEX = std::shared_mutex>
	//using ThreadSafeVariablePtr = std::shared_ptr<ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX>>;
	using ThreadSafeSharedVariable = ThreadSafeVariable<std::shared_ptr<T_VARIABLE_TYPE>, T_MUTEX>;
}