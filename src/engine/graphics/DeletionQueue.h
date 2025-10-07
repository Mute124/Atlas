/**************************************************************************************************
 * @file DeletionQueue.h
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once


#include <deque>
#include <functional>

namespace Atlas {
	class DeletionQueue
	{
	private:

		std::deque<std::function<void()>> mDeletorsDeque;

	public:

		DeletionQueue() = default;

		~DeletionQueue() { 
			flush();
		}

		void push(std::function<void()>&& function) {
			mDeletorsDeque.push_back(function);
		}

		void flush() {
			// reverse iterate the deletion queue to execute all the functions
			for (auto it = mDeletorsDeque.rbegin(); it != mDeletorsDeque.rend(); it++) {
				(*it)(); //call functors
			}
			
			mDeletorsDeque.clear();
		}
		
		size_t size() const {
			return mDeletorsDeque.size();
		}

		std::deque<std::function<void()>>& getDeletorsDeque() {
			return mDeletorsDeque;
		}

	};
}
