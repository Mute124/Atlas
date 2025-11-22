/**************************************************************************************************
 * @file EffectManager.cpp
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "EffectManager.h"
#include "../../debugging/Logging.h"

void Atlas::EffectManager::pushEffect(ComputeEffect& effect) {
	int index = mComputeEffects.size();
	InfoLog(std::format("Pushing effect: {} at index: {}", effect.name, index));

	effect.index = index;
	mComputeEffects.push_back(effect);

	mValidEffectIndices.emplace(index);
}

bool Atlas::EffectManager::validEffectExistsAt(size_t index)
{
	return mValidEffectIndices.contains(index);
}

int& Atlas::EffectManager::getCurrentEffectIndex() {
	return mCurrentEffectIndex;
}

int Atlas::EffectManager::getEffectCount() {
	return mComputeEffects.size();
}

Atlas::ComputeEffect & Atlas::EffectManager::getEffect(int index)
{
	int finalIndex = index;
	if (finalIndex > mComputeEffects.size()) {
		const bool cbIndexZeroHasValidEffect = validEffectExistsAt(0);
		std::string errorMessage;

		if (cbIndexZeroHasValidEffect) {
			errorMessage = std::format("Effect index out of range: {}. Returning first effect at index 0.", mCurrentEffectIndex);
			finalIndex = 0;
			mCurrentEffectIndex = 0;
		}
		else {
			errorMessage = std::format("Effect index out of range: {}, and no valid effect exists at index 0!", mCurrentEffectIndex);
		}

		ErrorLog(errorMessage);

		//index = 0;
	}

	return mComputeEffects[finalIndex];
}

Atlas::ComputeEffect& Atlas::EffectManager::getCurrentEffect() {
	int index = mCurrentEffectIndex;

	if (index > mComputeEffects.size()) {
		const bool cbIndexZeroHasValidEffect = validEffectExistsAt(0);
		std::string errorMessage;

		if (cbIndexZeroHasValidEffect) {
			errorMessage = std::format("Effect index out of range: {}. Returning first effect at index 0.", index);
			index = 0;
			mCurrentEffectIndex = 0;
		}
		else {
			errorMessage = std::format("Effect index out of range: {}, and no valid effect exists at index 0!", index);
		}

		ErrorLog(errorMessage);

		//index = 0;
	}

	return mComputeEffects[index];
}
