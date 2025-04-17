/**************************************************************************************************
 * @file PacedThread.h
 * 
 * @brief 
 * 
 * @date April 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include "GameThread.h"

namespace Atlas {

	class PacedThread : public GameThread {
	private:
		std::atomic<bool> mIsPaced;
	public:
	

	};
}