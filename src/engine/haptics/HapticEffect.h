/**************************************************************************************************
 * @file HapticEffect.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <cstdint>
#include <memory>

#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_stdinc.h>

#define ATLAS_BAD_HAPTIC_EFFECT_ID -1

namespace Atlas {
	using HapticEffectInfo = SDL_HapticEffect;

	class HapticDevice;

	struct MonoRumbleHapticEffectInfo {
		float strength = 1.0f;
		uint32_t lengthMS = 2000; // one second
	};

	struct HapticEffectUploadResult {
		int effectID = ATLAS_BAD_HAPTIC_EFFECT_ID;
		bool bSuccess = false;
		bool bIgnoreBadEffectID = false;
	};

	class IHapticEffect {
	public:

		virtual ~IHapticEffect() = default;

		virtual void play(std::shared_ptr<HapticDevice> hapticDevice) = 0;

		virtual void stop(std::shared_ptr<HapticDevice> hapticDevice) = 0;

		virtual void pause(std::shared_ptr<HapticDevice> hapticDevice) = 0;

		virtual void resume(std::shared_ptr<HapticDevice> hapticDevice) = 0;

		virtual HapticEffectUploadResult upload(std::shared_ptr<HapticDevice> hapticDevice) = 0;

		virtual void destroy(std::shared_ptr<HapticDevice> hapticDevice) = 0;
	};

	class MonoRumbleHapticEffect : public IHapticEffect {
	private:

		MonoRumbleHapticEffectInfo mRumbleHapticEffectInfo;

		bool mbIsUploaded = false;
	public:

		MonoRumbleHapticEffect() = default;

		// Inherited via IHapticEffect
		void play(std::shared_ptr<HapticDevice> hapticDevice) override;

		void stop(std::shared_ptr<HapticDevice> hapticDevice) override;

		void pause(std::shared_ptr<HapticDevice> hapticDevice) override;

		void resume(std::shared_ptr<HapticDevice> hapticDevice) override;

		HapticEffectUploadResult upload(std::shared_ptr<HapticDevice> hapticDevice) override;

		void destroy(std::shared_ptr<HapticDevice> hapticDevice) override;
	};

	class HapticEffect {
	private:
		HapticEffectInfo mHapticEffect;
		
		/**
		 * @brief The id of the haptic effect @b AFTER it has been uploaded.
		 * 
		 * @since v0.0.1
		 */
		int mEffectID = 0;
	
		bool mbIsUploaded = false;
	public:
		
		explicit HapticEffect(HapticEffectInfo const& hapticEffect) : mHapticEffect(hapticEffect) {}

		HapticEffect() = default;

		void setHapticEffect(HapticEffectInfo const& hapticEffect) {
			mHapticEffect = hapticEffect;
		}

		//int upload(std::shared_ptr<HapticDevice> hapticDevice) {
		//	mEffectID = SDL_HapticNewEffect(hapticDevice->getSDLHapticPtr().get(), &mHapticEffect);
		//	mbIsUploaded = true;
		//}
	};
}
