#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <functional>
namespace Atlas {

	/**
	 * @brief A managed variable is one that is thread-safe, handled by @ref MemoryAllocator, and can be used to share data between threads. 
	 * @note This is not used yet
	 * @tparam T_VARIABLE_TYPE What is the type of the variable
	 */
/*	template<typename T_VARIABLE_TYPE, typename T_MUTEX = std::mutex, typename T_ALLOCATOR>
	class IManagedVariable {
	protected:
		T_VARIABLE_TYPE mVariable;
		T_MUTEX mMutex;
	public:

		virtual T_VARIABLE_TYPE get() = 0;
		virtual void set(const T_VARIABLE_TYPE& variable) = 0;
		virtual void lock() = 0;
		virtual bool isLocked() = 0;
	};
	

	template<typename T_VARIABLE_TYPE, typename T_MUTEX = std::mutex, typename T_ALLOCATOR>
	class BManagedVariable : public IManagedVariable<T_VARIABLE_TYPE, T_MUTEX, T_ALLOCATOR> {
	private:

	public:
		virtual T_VARIABLE_TYPE get() {

		}

		virtual void set(const T_VARIABLE_TYPE& variable) {
		}

		virtual void lock() {
		}

		virtual bool isLocked() {
		}

		virtual void execute(std::function<void(ManagedVariable<T_VARIABLE_TYPE, T_MUTEX, T_ALLOCATOR>&)> function) {
			std::unique_lock lock1{ mMutex, std::defer_lock };

			std::lock(lock1);

			function(*this);
		}
	};*/

	/**
	 * @brief This class wraps a variable (specified by T_VARIABLE_TYPE) and makes it thread-safe. When @ref ThreadSafeVariable::execute()
	 * is called, it will lock this variable and execute the function that is passed to it.
	 * @tparam T_VARIABLE_TYPE The type of variable to wrap
	 * @tparam T_MUTEX What type of mutex to use. This defaults to std::mutex
	 * @tparam T_LOCK_GUARD What type of lock_guard to use. This defaults to std::lock_guard<T_MUTEX>
	 * @since v0.0.9
	 */
	template <typename T_VARIABLE_TYPE, typename T_MUTEX = std::mutex, typename T_LOCK_GUARD = std::lock_guard<T_MUTEX>>
	class ThreadSafeVariable {
	private:

		/**
		 * @brief This is the variable that is wrapped by this class. 
		 * @since v0.0.9
		 */
		T_VARIABLE_TYPE value;
		T_MUTEX mutex;
	public:
		ThreadSafeVariable(const T_VARIABLE_TYPE& initialValue) : value(initialValue) {}
		ThreadSafeVariable() = default;

		// copy constructor
		ThreadSafeVariable(const ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX, T_LOCK_GUARD>& other) {
			T_LOCK_GUARD lock(mutex);
			value = other.value;
		}

		T_VARIABLE_TYPE get() {

			return value;
		}

		void set(const T_VARIABLE_TYPE& newValue) {
			T_LOCK_GUARD lock(mutex);
			value = newValue;
		}

		void execute(std::function<void(ThreadSafeVariable<T_VARIABLE_TYPE>&)> function) {
			std::unique_lock lock1{ mutex, std::defer_lock };
			T_LOCK_GUARD lock(mutex);

			function(*this);
		}
		ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX, T_LOCK_GUARD>& operator=(
				const ThreadSafeVariable<T_VARIABLE_TYPE, T_MUTEX, T_LOCK_GUARD>& other) {
			T_LOCK_GUARD lock(mutex);
			//value = other.value;
			this = other;
			return *this;
		}

		T_VARIABLE_TYPE operator()() {
			return get();
		}


	};

}