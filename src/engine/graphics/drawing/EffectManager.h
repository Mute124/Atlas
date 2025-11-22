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
#include <unordered_set>
#include "Effect.h"


namespace Atlas {
	class EffectManager {
		friend class VulkanRenderingBackend;
		friend class RenderPass;
	private:
		std::vector<ComputeEffect> mComputeEffects;
		std::unordered_set<int> mValidEffectIndices;

		
	public:
		int mCurrentEffectIndex{ 0 };
		void pushEffect(ComputeEffect& effect);

		bool validEffectExistsAt(size_t index);

		int& getCurrentEffectIndex();

		int* getCurrentEffectIndexPtr() { return &mCurrentEffectIndex; }

		int getEffectCount();

		ComputeEffect& getEffect(int index);
		ComputeEffect& getCurrentEffect();


	};
}
