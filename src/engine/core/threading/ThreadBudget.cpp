/**************************************************************************************************
 * @file ThreadBudget.cpp
 * 
 * @brief Implementation of the @ref ThreadBudget class.
 * 
 * @date January 2026
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#include "ThreadBudget.h"
#include <stdexcept>

Atlas::ThreadBudget::ThreadBudget(uint8_t budget, uint8_t budgetUsed, uint8_t budgetRemaining) : mBudget(budget), mBudgetUsed(budgetUsed), mBudgetRemaining(budgetRemaining) {
	if (budgetUsed > budget) throw std::runtime_error("Budget used value is greater than the budget passed.");
	if (budgetRemaining > budget) throw std::runtime_error("Budget remaining value is greater than the budget passed.");
}

Atlas::ThreadBudget::ThreadBudget(uint8_t budget, uint8_t budgetUsed) : ThreadBudget(budget, budgetUsed, budget - budgetUsed) {}

Atlas::ThreadBudget::ThreadBudget(uint8_t budget)
	: ThreadBudget(budget, 0, budget) {}

void Atlas::ThreadBudget::setBudget(uint8_t newBudget) {
	mBudget = newBudget;
	this->refreshBudget();
}

void Atlas::ThreadBudget::addBudget(uint8_t amount) {
	mBudget += amount;
	mBudgetRemaining += amount;
}

void Atlas::ThreadBudget::useBudget(uint8_t amount) {
	if (amount > mBudgetRemaining) {
		throw std::runtime_error("Attempted to allocate a value higher than the remaining budget.");
	}

	mBudgetUsed += amount;
	mBudgetRemaining -= amount;
}

void Atlas::ThreadBudget::freeBudget(uint8_t amount) {
	if (amount > mBudget) {
		throw std::runtime_error("Attempted to free a value higher than the budget.");
	}

	mBudgetUsed -= amount;
	mBudgetRemaining += amount;
}

void Atlas::ThreadBudget::refreshBudget() {
	mBudgetRemaining = mBudget - mBudgetUsed;
}

void Atlas::ThreadBudget::resetBudget() {
	mBudgetUsed = 0;
	mBudgetRemaining = mBudget;
}

uint8_t Atlas::ThreadBudget::getBudget() const {
	return mBudget;
}

uint8_t Atlas::ThreadBudget::getBudgetUsed() const {
	return mBudgetUsed;
}

uint8_t Atlas::ThreadBudget::getBudgetRemaining() const {
	return mBudgetRemaining;
}
