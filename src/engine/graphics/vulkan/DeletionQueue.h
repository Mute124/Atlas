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
	using DeletionQueueDeletor = std::function<void()>;

	class DeletionQueue
	{
	private:

		std::deque<DeletionQueueDeletor> mDeletorsDeque;

	public:

		DeletionQueue() = default;

		~DeletionQueue() { 
			flush();
		}

		void push(DeletionQueueDeletor&& function) {
			mDeletorsDeque.push_back(function);
		}

		void flush() {

			if (!mDeletorsDeque.empty()) {
				// reverse iterate the deletion queue to execute all the functions
				for (auto it = mDeletorsDeque.rbegin(); it != mDeletorsDeque.rend(); it++) {
					(*it)(); //call functors
				}
			}

			mDeletorsDeque.clear();
		}
		
		size_t getSize() const {
			return mDeletorsDeque.size();
		}

		std::deque<DeletionQueueDeletor>& getDeletorsDeque() {
			return mDeletorsDeque;
		}
	};
}
