/**************************************************************************************************
 * @file EffectManager.h
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <vector>

#include "Effect.h"


namespace Atlas {
	class EffectManager {
	private:
		std::vector<ComputeEffect> mComputeEffects;

		int mCurrentEffectIndex{ 0 };
	public:

		int getCurrentEffectIndex() {
			return mCurrentEffectIndex;
		}

		int getEffectCount() {
			return mComputeEffects.size();
		}

		ComputeEffect& getCurrentEffect() {
			return mComputeEffects[mCurrentEffectIndex];
		}
	};
}
