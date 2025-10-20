/**************************************************************************************************
 * @file HapticEffect.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "HapticEffect.h"
#include "HapticDevice.h"

#include "../debugging/Logging.h"

void Atlas::MonoRumbleHapticEffect::play(std::shared_ptr<HapticDevice> hapticDevice)
{
    SDL_HapticRumblePlay(hapticDevice->getSDLHapticPtr().get(), mRumbleHapticEffectInfo.strength, mRumbleHapticEffectInfo.lengthMS);
}

void Atlas::MonoRumbleHapticEffect::stop(std::shared_ptr<HapticDevice> hapticDevice)
{
    SDL_HapticRumbleStop(hapticDevice->getSDLHapticPtr().get());
}

void Atlas::MonoRumbleHapticEffect::pause(std::shared_ptr<HapticDevice> hapticDevice)
{
    stop(hapticDevice);
}

void Atlas::MonoRumbleHapticEffect::resume(std::shared_ptr<HapticDevice> hapticDevice)
{
	play(hapticDevice);
}

Atlas::HapticEffectUploadResult Atlas::MonoRumbleHapticEffect::upload(std::shared_ptr<HapticDevice> hapticDevice)
{
    HapticEffectUploadResult uploadResult{};

    if (!SDL_HapticRumbleSupported(hapticDevice->getSDLHapticPtr().get())) {
        const std::string cUnsupportedTraceMsg = std::format(
			"The haptic device with the name of {} (id: {}) does not support rumble effects!",
			SDL_HapticName(hapticDevice->getHapticDeviceID()),
			hapticDevice->getHapticDeviceID()
		);

        WarnLog(cUnsupportedTraceMsg);
        // The value of bSuccess is already false, so it doesn't need to be set. 
		return uploadResult;
    }

    // Tell the caller to ignore the bad effect ID because the rumble effect works differently.
    // Additionally, this is set here rather than before the check to see if the effect is supported
    // because the HapticDevice object will ignore the fact that it is not supported!
    uploadResult.bIgnoreBadEffectID = true; 

    if (SDL_HapticRumbleInit(hapticDevice->getSDLHapticPtr().get()) == 0) {
        uploadResult.bSuccess = true;
        
        mbIsUploaded = true;
    }

    return uploadResult;
}

void Atlas::MonoRumbleHapticEffect::destroy(std::shared_ptr<HapticDevice> hapticDevice)
{

}
