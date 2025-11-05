/**************************************************************************************************
 * @file HapticDevice.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <memory>
#include <cstdint>
#include <stdexcept>
#include <format>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_error.h>

#include "HapticDevice.h"
#include "../debugging/Logging.h"



/**************************************************************************************************
* HapticDeviceInitInfo struct definitions
**************************************************************************************************/

Atlas::HapticDeviceInitInfo::HapticDeviceInitInfo(HapticDeviceID hapticDeviceID) : initMethod(EHapticDeviceInitMethod::Id), initInfo(hapticDeviceID) 
{
}

Atlas::HapticDeviceInitInfo::HapticDeviceInitInfo(SDL_Joystick* joystick) : initMethod(EHapticDeviceInitMethod::Joystick), initInfo(joystick) 
{
}

/**************************************************************************************************
* HapticDevice class definitions
* **************************************************************************************************/

std::shared_ptr<SDL_Haptic> Atlas::HapticDevice::openHapticDeviceFromID(HapticDeviceID hapticDeviceID) const
{
	return nullptr; //std::make_shared<SDL_Haptic>(SDL_HapticOpen(hapticDeviceID));
}

std::shared_ptr<SDL_Haptic> Atlas::HapticDevice::openHapticDeviceFromJoystick(SDL_Joystick* joystick) const
{
	return nullptr;//std::make_shared<SDL_Haptic>(SDL_HapticOpenFromJoystick(joystick));
}

std::shared_ptr<SDL_Haptic> Atlas::HapticDevice::openHapticDeviceFromMouse() const
{
	return nullptr;//std::make_shared<SDL_Haptic>(SDL_HapticOpenFromMouse());
}

Atlas::HapticDevice::HapticDevice(HapticDeviceID hapticDeviceID) 
	: mHapticDeviceID(hapticDeviceID)
{
}

Atlas::HapticDevice::~HapticDevice()
{
	close();
}

inline uint8_t Atlas::HapticDevice::GetMaxHapticDevices()
{
	return sMaxHapticDevices;
}

inline void Atlas::HapticDevice::SetMaxHapticDevices(uint8_t maxHapticDevices)
{
}

inline uint8_t Atlas::HapticDevice::GetHapticDevicesCount()
{
	return sCurrentHapticDevicesCount;
}

inline bool Atlas::HapticDevice::HasReachedMaxHapticDevices()
{
	return GetMaxHapticDevices() != -1 && GetHapticDevicesCount() >= GetMaxHapticDevices();
}

void Atlas::HapticDevice::open()
{
	// Make sure that the haptic device is not already open because that can cause issues.
	if (mbIsOpen == true) {
		const std::string cAlreadyOpenMsg = std::format(
			"Haptic device with the name of {} (id: {}) is already open. This haptic device will be closed and opened again.",
			SDL_HapticName(getHapticDeviceID()),
			getHapticDeviceID()
		);
		
		//"Trying to open a haptic device that is already open!"
		WarnLog(cAlreadyOpenMsg);
		
		close();
	}

	// TODO: This may cause an issue because this may or may not open a haptic device because of the comparator used in the if statement.
	else if (HasReachedMaxHapticDevices()) {
		const std::string cMaxDevicesReachedMsg = std::format(
			"Max haptic devices reached. The haptic device with the name of {} (id: {}) will not be opened.",
			SDL_HapticName(getHapticDeviceID()),
			getHapticDeviceID()
		);
		
		//"Max haptic devices reached. This haptic device will not be opened."
		WarnLog(cMaxDevicesReachedMsg);
		//throw std::runtime_error("Max haptic devices reached.");
	}
	
	
	mSDLHapticPtr = std::shared_ptr<SDL_Haptic>(SDL_HapticOpen(getHapticDeviceID()));
	
	if (!SDL_HapticOpened(getHapticDeviceID())) {

		const std::string cSdlErrorMsg = SDL_GetError();

		const std::string cFailureMsg = std::format(
			"Failed to open haptic device with the name of {} (id: {}) because of the following error: {}.",
			SDL_HapticName(getHapticDeviceID()),
			getHapticDeviceID(),
			cSdlErrorMsg
		);

		ErrorLog(cFailureMsg);
		//throw std::runtime_error("Failed to open haptic device.");
	}

	/*if (mSDLHapticPtr == nullptr) {
		throw std::runtime_error("Failed to open haptic device.");
	}*/

	const std::string cSuccessfulOpenMsg = std::format(
		"Successfully opened haptic device with the name of {} (id: {}).",
		SDL_HapticName(getHapticDeviceID()),
		getHapticDeviceID()
	);

	TraceLog(cSuccessfulOpenMsg);

	mbIsOpen = true;
	sCurrentHapticDevicesCount++;


	////SDL_Delay(2000);
}

void Atlas::HapticDevice::close()
{
	if (mbIsOpen == true) {
		const std::string cCloseAttemptTraceMsg = std::format(
			"Closing haptic device with the name of {} (id: {}).",
			SDL_HapticName(getHapticDeviceID()),
			getHapticDeviceID()
		);

		TraceLog(cCloseAttemptTraceMsg);

		SDL_HapticClose(mSDLHapticPtr.get());
		mbIsOpen = false;
		sCurrentHapticDevicesCount--;
	}
}

void Atlas::HapticDevice::addHapticEffect(std::unique_ptr<IHapticEffect> hapticEffect)
{
	//HapticEffectUploadResult uploadResult = hapticEffect->upload(shared_from_this());
	//
	//int emplaceLocation = 0;

	//if (uploadResult.bIgnoreBadEffectID
	//	&& uploadResult.effectID == ATLAS_BAD_HAPTIC_EFFECT_ID
	//	&& uploadResult.bSuccess) 
	//{
	//	// This means that we are dealing with a MonoRumbleHapticEffect or something that does not run in the 
	//	// same way as other haptic effects.
	//	emplaceLocation = mHapticEffects.size();
	//	mHapticEffects.emplace(emplaceLocation, std::move(hapticEffect));
	//}
	//else if (uploadResult.bSuccess) {
	//	mHapticEffects
	//}
	
}

bool Atlas::HapticDevice::isOpen() const { 
	return mbIsOpen;
}

bool Atlas::HapticDevice::canUseEffect(std::unique_ptr<IHapticEffect> hapticEffect) const
{
	return false;
}

std::shared_ptr<SDL_Haptic> Atlas::HapticDevice::getSDLHapticPtr() const {
	return mSDLHapticPtr;
}

Atlas::HapticDeviceID Atlas::HapticDevice::getHapticDeviceID() const { 
	return mHapticDeviceID;
}

Atlas::HapticDevice::operator std::string() const
{
	return static_cast<std::string>(operator const char *());
}

Atlas::HapticDevice::operator const char* () const
{
	return SDL_HapticName(getHapticDeviceID());
}


