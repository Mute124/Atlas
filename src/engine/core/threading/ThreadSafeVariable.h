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
#include <functional>
#include <type_traits>

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
	template <typename T_VARIABLE_TYPE, typename T_MUTEX = std::mutex, typename T_LOCK_GUARD = std::lock_guard<T_MUTEX>>
	class ThreadSafeVariable {
	private:

		/**
		 * @brief This is the variable that is wrapped by this class. 
		 * 
		 * @since v0.0.9
		 */
		T_VARIABLE_TYPE mValue;
		T_MUTEX mMutex;

	public:

		/**
		 * @brief 
		 */
		//class MissingDefaultConstructorException : public InvalidArgument {
		//public:
		//	using InvalidArgument::InvalidArgument;
		//};

		/**
		 * @brief The default constructor. This will first check if the variable's
		 * type (T_VARIABLE_TYPE) can be default constructed. If it can, it will 
		 * do so. If it cannot, it will throw an exception that the variable
		 * cannot be constructed because this class has no idea how to construct it.
		 * 
		 * @remarks This constructor is useful for initializing the variable. 
		 * Below is an example of what @b NOT to do when using this constructor
		 * 
		 * @code 
		 * class MyClass {
		 * public: 
		 *		int x;
		 * 
		 *		MyClass() = delete;
		 * };
		 * 
		 * // in a function
		 * ThreadSafeVariable<MyClass> x;
		 * @endcode
		 * 
		 * @warning If the variable cannot be default constructed, an exception
		 * will be thrown!
		 * 
		 * @throws MissingDefaultConstructorException if the variable does not
		 * have a default constructor
		 *
		 * @since v0.0.9
		 */
		ThreadSafeVariable() {
			// if the variable can be default constructed, do it
			if (std::is_default_constructible<T_VARIABLE_TYPE>::value) {
				mValue = T_VARIABLE_TYPE{};
			}
			else {
				//throw MissingDefaultConstructorException("The variable cannot be default constructed!");
			}
		}


		/**
		 * @brief This @b EXPLICIT constructor takes a variable of type T_VARIABLE_TYPE
		 * and sets mValue to the value of that variable. Keep in mind that this is a
		 * copy constructor for the wrapped variable. While setting the variable, this
		 * operation will lock the mutex.
		 * 
		 * @remarks This constructor is useful for initializing the variable with a value.
		 * For example, if you have a variable called x that you want to initialize to 5,
		 * you can do so like so:
		 * 
		 * @code 
		 * ThreadSafeVariable<int> x(5);
		 * @endcode
		 * 
		 * or like so:
		 * 
		 * @code
		 * ThreadSafeVariable<int> x = ThreadSafeVariable<int>(5);
		 * @endcode
		 * 
		 * @param initialValue A unary const reference to a variable of type T_VARIABLE_TYPE
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref mValue for the documentation on the variable that is wrapped by this class
		 */
		explicit ThreadSafeVariable(T_VARIABLE_TYPE const& initialValue) {
			set(initialValue);
		}

		/**
		 * @brief The class's copy constructor that takes another ThreadSafeVariable
		 * as a parameter, and copies the variable wrapped by that class. Keep in mind
		 * that this operation is thread-safe because it locks the mutex.
		 * 
		 * @note This is the preferred way to copy a ThreadSafeVariable.
		 * 
		 * @param other A unary const reference to another ThreadSafeVariable @b WITH 
		 * the same template parameters as this class. If it does not have the same
		 * template parameters, it will not compile.
		 * 
		 * @since v0.0.9
		 */
		ThreadSafeVariable(ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX, T_LOCK_GUARD>& other) {
			set(other.get());
		}


		/**
		 * @brief This function returns the variable that is wrapped by this
		 * class (mValue). There is no need to lock the mutex in this function
		 * because it is not being modified, and it is being copied. Should 
		 * you need a reference to the variable, you can use the getAsReference()
		 * function. 
		 * 
		 * @return A copy of the variable that is wrapped by this class. It
		 * is declared under T_VARIABLE_TYPE (the class's template parameter).
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref getAsReference() for the sister function.
		 * @sa @ref mValue for the variable that is wrapped by this class.
		 */
		T_VARIABLE_TYPE get() {
			return mValue;
		} 

		/**
		 * @brief Similar to get(), but returns a reference to the variable instead
		 * of simply copying it. Please refer to the remarks section below for info
		 * on why this function is not preferred.
		 *
		 * @remarks This function is not preferable to get() because
		 * there is a higher likelihood of a race condition or other thread safety
		 * issue.
		 * 
		 * @return A unary reference to the variable that is wrapped by this class.
		 * See the brief description for more info.
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref get() for the sister function.
		 * @sa @ref mValue for the variable that is wrapped by this class.
		 */
		T_VARIABLE_TYPE& getAsReference() {
			return mValue;
		}

		/**
		 * @brief This is one of the most important functions in this class 
		 * because it allows you to set the mValue of the variable without 
		 * having to deal with race conditions or locks. When this is called,
		 * it will automatically lock the mutex with whatever T_LOCK_GUARD
		 * is specified as within the template parameters. Then it will lock
		 * and then set the variable (mValue) to newValue.
		 * 
		 * @remarks As an example for the use of this function, let's say you
		 * have a variable called x that you want to set to 5. You can do
		 * this by calling set(5) within this class or you can do this by calling 
		 * x.set(5). Even if multiple threads are attempting to set the mValue,
		 * there will be @b NO race conditions. and the mValue will be uniform
		 * across all threads. 
		 * 
		 * @param newValue The new mValue to set the variable to. This is a const
		 * reference, so make sure to account for that. This variable will be 
		 * copied into the variable that is wrapped by this class.
		 * 
		 * @since v0.0.9
		 */
		void set(const T_VARIABLE_TYPE& newValue) {
			T_LOCK_GUARD lock(mMutex);
			mValue = newValue;
		}

		/**
		 * @brief Locks the mutex variable and returns a reference to 
		 * T_LOCK_GUARD.
		 * 
		 * @note This returns a reference to T_LOCK_GUARD because it would 
		 * automatically unlock the mutex when it goes out of scope if it
		 * wasn't returned as a reference.
		 * 
		 * @return A reference to T_LOCK_GUARD so that it doesn't
		 * automatically unlock the mutex. See the note above for
		 * the reasoning behind this.
		 */
		[[nodiscard]] T_LOCK_GUARD& lock() {
			T_LOCK_GUARD lock(mMutex);
			return lock;
		}

		/**
		 * @brief Check to see if the mutex is locked by calling
		 * std::mutex::try_lock(). If it is locked, it returns true,
		 * otherwise it returns false. The mValue returned is the opposite
		 * of what std::mutex::try_lock() returns. See the documentation
		 * (more specifically the note below) for why the opposite is returned.
		 * Additionally, See the remark below the note for info on how this 
		 * function can be useful for you. 
		 * 
		 * @note Instead of directly returning the mValue of try_lock(), the 
		 * opposite of it is returned instead because C++20 documentation 
		 * states that it will return @c true if the mutex was able to be 
		 * locked.
		 *
		 * @remarks This is quite useful for checking if the variable
		 * is currently in use somewhere. Let's say you have a function
		 * that you want to execute, but you don't want to execute it
		 * if the variable is already in use (so you dont waste time).
		 * You can use this function to check if the variable is in use,
		 * therefore saving time.
		 * 
		 * @return The result of an attempt to lock the mutex, @c true
		 * if it was locked, @c false otherwise.
		 * 
		 * @since v0.0.9
		 */
		bool isLocked() {
			return !mMutex.try_lock();
		}

		/**
		 * @brief Executes a lambda function while locking the mutex.
		 * 
		 * @param function 
		 */
		void execute(std::function<void(ThreadSafeVariable<T_VARIABLE_TYPE>&)> function) {
			std::unique_lock lock1{ mMutex, std::defer_lock };
			T_LOCK_GUARD lock(mMutex);

			function(*this);
		}

		/**
		 * @brief This is the copy assignment operator, it only copies the mValue of the variable,
		 * not the mutex. Keep in mind that this @b LOCKS
		 * @param other 
		 * @return 
		 */
		ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX, T_LOCK_GUARD>& operator=(
				const ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX, T_LOCK_GUARD>& other) {
			T_LOCK_GUARD lock(mMutex);
			mValue = other.mValue;
			//this = other;
			return *this;
		}

		/**
		 * @brief Equivalent to the get() function.
		 * @return 
		 */
		T_VARIABLE_TYPE operator()() {
			return get();
		}

		/**
		 * @brief This is equivalent to get() and operator()(). It also
		 * works in the same way that the -> operator in a shared_ptr works.
		 * 
		 * @return A copy of the wrapped variable
		 * 
		 * @since v0.0.9
		 */
		T_VARIABLE_TYPE operator->() {
			return get();
		}

	};

	template<typename T_VARIABLE_TYPE>
	using ThreadSafeSharedVariable = ThreadSafeVariable<std::shared_ptr<T_VARIABLE_TYPE>>;

}