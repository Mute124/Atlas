/**************************************************************************************************
 * @file ThreadBudget.h
 * 
 * @brief Provides a thread budget that can be used in a multi-threaded environment.
 * 
 * @date January 2026
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once
#include <cstdint>

namespace Atlas {
	
	/**
	 * @brief A class that represents a thread budget. It can be used to limit the amount of threads 
	 * that can be used in a multi-threaded environment. Alternatively, an edge case of this is that
	 * it can be used to limit the amount of tasks that can be executed in a multi-threaded environment
	 * or just provide a "budget" for something.
	 * 
	 * @note This class cannot be inherited.
	 * 
	 * @todo Refactor this so that it will throw custom exceptions instead of generic std::runtime_error
	 * to make it more maintainable.
	 * 
	 * @since v0.0.1
	 */
	class ThreadBudget final {
	private:
		/**
		 * @brief The maximum amount of threads that can be used.
		 * 
		 * @since v0.0.1
		 */
		uint8_t mBudget;

		/**
		 * @brief The amount of threads that have been used, hence the amount of the budget that has
		 * been used.
		 * 
		 * @since v0.0.1
		 */
		uint8_t mBudgetUsed{ 0 };

		/**
		 * @brief The remaining amount of threads that are left in the budget.
		 * 
		 * @since v0.0.1
		 */
		uint8_t mBudgetRemaining;

	public:

		/**
		 * @brief Constructs a new ThreadBudget with all of the member variables within the class.
		 * 
		 * @note This constructor is not preferred compared to the other constructors. However,
		 * this constructor exists for the sake of completeness.
		 * 
		 * @throws std::runtime_error if:
		 * 1. The budget used value is greater than the budget passed.
		 * 2. The budget remaining value is greater than the budget passed.
		 * 
		 * @param budget The maximum amount of threads that can be used.
		 * @param budgetUsed The amount of threads that have been used.
		 * @param budgetRemaining The remaining amount of threads that are left in the budget.
		 * 
		 * @since v0.0.1
		 */
		ThreadBudget(uint8_t budget, uint8_t budgetUsed, uint8_t budgetRemaining);

		/**
		 * @brief Constructs a new ThreadBudget. Use this constructor if you have already used part of a
		 * budget.
		 * 
		 * @throws std::runtime_error Due to the use of the full ThreadBudget constructor, this will throw
		 * if the checks in the full ThreadBudget constructor are not met. Please see the full constructor.
		 * 
		 * @param budget The maximum amount of threads that can be used.
		 * @param budgetUsed The amount of threads that have been used.
		 * 
		 * @since v0.0.1
		 */
		ThreadBudget(uint8_t budget, uint8_t budgetUsed);

		/**
		 * @brief Constructs a new ThreadBudget. This is what you should use to create a new ThreadBudget
		 * most of the time.
		 * 
		 * @throws std::runtime_error Due to the use of the full ThreadBudget constructor, this will throw
		 * if the checks in the full ThreadBudget constructor are not met. Please see the full constructor.
		 * 
		 * @param budget The maximum amount of threads that can be used.
		 * 
		 * @since v0.0.1
		 */
		explicit ThreadBudget(uint8_t budget);

		
		ThreadBudget() = default;
		~ThreadBudget() = default;

		/**
		 * @brief Sets the value of mBudget to a specific amount and refreshes the budget.
		 * 
		 * @param newBudget The new value of mBudget
		 * 
		 * @since v0.0.1
		 */
		void setBudget(uint8_t newBudget);

		/**
		 * @brief Adds a specific amount to mBudget. Use this if you want to increase the budget without
		 * constructing a new ThreadBudget.
		 * 
		 * @param amount The amount to add to mBudget.
		 * 
		 * @since v0.0.1
		 */
		void addBudget(uint8_t amount);

		/**
		 * @brief Uses (subtracts) a specific amount of the budget.
		 * 
		 * @throws std::runtime_error if the amount passed is greater than the remaining budget
		 * 
		 * @param amount The amount to subtract from the budget
		 * 
		 * @since v0.0.1
		 */
		void useBudget(uint8_t amount);

		/**
		 * @brief Frees a specific amount of the budget, thereby increasing the remaining budget.
		 * 
		 * @throws std::runtime_error if the amount passed is greater than the used budget
		 * 
		 * @param amount The amount to free
		 * 
		 * @since v0.0.1
		 */
		void freeBudget(uint8_t amount);

		/**
		 * @brief Refreshes the value of mBudgetRemaining by subtracting mBudgetUsed from mBudget.
		 * 
		 * @since v0.0.1
		 */
		void refreshBudget();

		/**
		 * @brief Resets the budget to its original value at the time of construction.
		 * 
		 * @since v0.0.1
		 */
		void resetBudget();

		/**
		 * @brief Returns the value of mBudget.
		 * 
		 * @return The value of mBudget
		 * 
		 * @since v0.0.1
		 */
		uint8_t getBudget() const;

		/**
		 * @brief Returns the value of mBudgetUsed.
		 * 
		 * @return The value of mBudgetUsed
		 * 
		 * @since v0.0.1
		 */
		uint8_t getBudgetUsed() const;

		/**
		 * @brief Returns the value of mBudgetRemaining.
		 * 
		 * @return The value of mBudgetRemaining
		 * 
		 * @since v0.0.1
		 */
		uint8_t getBudgetRemaining() const;
	};
}
