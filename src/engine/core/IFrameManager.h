/*****************************************************************//**
 * @file   IFrameManager.h
 * 
 * @brief  
 * 
 * @date   October 2024
 * 
 * @since 
 *********************************************************************/
#pragma once
#include <cmath>

#include "ThreadGroup.h"


#define ATLAS_THREAD_YIELD std::this_thread::yield()

namespace Atlas {

#ifndef ATLAS_BARE_BONES

	class FrameManager : public ThreadGroup {
	private:
		/**
		 * @brief The percentage of threads that Atlas is
		 * allowed to use. The default value is 0.5 (50%).
		 * 
		 * @since v0.0.2
		 */
		float mMaxUtilizationPercentage = 0.5f;

		const size_t mMaxThreads = static_cast<size_t>(round(std::thread::hardware_concurrency() * mMaxUtilizationPercentage));
	public:

		FrameManager() : ThreadGroup() {}
	};
#endif
}