/**************************************************************************************************
 * @file HapticDeviceEffectManager.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <memory>
#include <type_traits>

#include "HapticDeviceEffectManager.h"
#include "HapticEffect.h"

//
//void Atlas::HapticDeviceEffectManager::push(int hapticEffectID, std::unique_ptr<HapticEffect> effect)
//{
//	if (!mHapticEffectsMap.contains(hapticEffectID)) {
//		mHapticEffectsMap[hapticEffectID] = std::move(effect);
//	}
//}
//
//void Atlas::HapticDeviceEffectManager::destroy(int hapticEffectID)
//{
//	if (mHapticEffectsMap.contains(hapticEffectID)) {
//		mHapticEffectsMap[hapticEffectID]->destroy();
//
//		mHapticEffectsMap.erase(hapticEffectID);
//	}
//}
