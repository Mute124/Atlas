/**************************************************************************************************
 * @file HapticDevice.h
 * 
 * @brief The HapticDevice class and its related functions.
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <memory>
#include <cstdint>
#include <variant>
#include <string>
#include <unordered_map>

#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_joystick.h>

#include "HapticEffect.h"

#define ATLAS_INVALID_HAPTIC_DEVICE_ID -1

namespace Atlas {
	/**
	 * @brief The ID of a haptic device. SDL uses an int to identify a haptic device.
	 *
	 * @since v0.0.1
	 */
	using HapticDeviceID = int;

	enum class EHapticDeviceInitMethod : int {
		Id = 0,
		Joystick = 1,
		Mouse = 2
	};

	struct HapticDeviceInitInfo {
		EHapticDeviceInitMethod initMethod = EHapticDeviceInitMethod::Id;
		
		std::variant<HapticDeviceID, SDL_Joystick*> initInfo;

		explicit HapticDeviceInitInfo(HapticDeviceID hapticDeviceID);
	
		explicit HapticDeviceInitInfo(SDL_Joystick* joystick);

		HapticDeviceInitInfo() = default;
		
	};

	/**
	 * @brief This class represents a physical device that can provide haptic feedback to the user.
	 * Due to the sheer amount of ways haptic feedback can be provided, this class acts as a wrapper
	 * for the SDL_Haptic object. 
	 * 
	 * @pre As is with the rest of Atlas' haptic API, SDL must be initialized before this class can be used and the flag
	 * SDL_INIT_HAPTIC must be set.
	 * 
	 * @since v0.0.1
	 */
	class HapticDevice : public std::enable_shared_from_this<HapticDevice> {
	private:
		/**
		 * @brief The maximum number of haptic devices that can be open at once. Set this to -1 to have no limit (default).
		 * 
		 * @since v0.0.1
		 */
		static inline uint8_t sMaxHapticDevices = -1;

		/**
		 * @brief The current number of haptic devices that are open. Anytime a haptic device is opened, this number will
		 * increase by 1, and any time a haptic device is closed, this number will decrease by 1.
		 * 
		 * @since v0.0.1
		 */
		static inline uint8_t sCurrentHapticDevicesCount = 0;
		
		std::unordered_map<int, std::unique_ptr<IHapticEffect>> mHapticEffects;

		/**
		 * @brief The SDL_Haptic object that this class wraps around. Since this is a shared pointer, it can be shared
		 * between multiple haptic devices if desired.
		 * 
		 * @note This will be null if the haptic device is not open. Additionally, this must be set to a valid SDL_Haptic
		 * value or else this class will not work.
		 * 
		 * @since v0.0.1
		 */
		std::shared_ptr<SDL_Haptic> mSDLHapticPtr = nullptr;

		/**
		 * @brief The ID of a valid physical haptic device that this class will represent. The value of this should be set
		 * in the constructor.
		 * 
		 * @since v0.0.1
		 */
		HapticDeviceID mHapticDeviceID = ATLAS_INVALID_HAPTIC_DEVICE_ID;

		/**
		 * @brief Whether or not this haptic device has been opened.
		 * 
		 * @since v0.0.1
		 */
		bool mbIsOpen = false;

		std::shared_ptr<SDL_Haptic> openHapticDeviceFromID(HapticDeviceID hapticDeviceID) const;
		std::shared_ptr<SDL_Haptic> openHapticDeviceFromJoystick(SDL_Joystick* joystick) const;
		std::shared_ptr<SDL_Haptic> openHapticDeviceFromMouse() const;
	public:
		
		/**
		 * @brief A constructor that sets the haptic device ID to the one provided. This must be a valid haptic device
		 * ID or else this class will not behave as expected and errors will occur.
		 * 
		 * @param hapticDeviceID The ID of the physical haptic device that this class will represent.
		 * 
		 * @since v0.0.1
		 */
		explicit HapticDevice(HapticDeviceID hapticDeviceID);

		/**
		 * @brief A default constructor.
		 * 
		 * @since v0.0.1
		 */
		HapticDevice() = default;

		/**
		 * @brief The default destructor that will close the haptic device if it is open.
		 * 
		 * @since v0.0.1
		 */
		~HapticDevice();

		/**
		 * @brief Returns the maximum number of haptic devices that can be open at once.
		 * 
		 * @returns @c sMaxHapticDevices
		 * 
		 * @since v0.0.1
		 */
		static uint8_t GetMaxHapticDevices();

		/**
		 * @brief Sets the maximum number of haptic devices that can be open at once.
		 * 
		 * @note If the value of @c sMaxHapticDevices is set to -1, then there will be no limit.
		 * 
		 * @param maxHapticDevices An 8-bit integer value that @c sMaxHapticDevices will be set to.
		 * 
		 * @since v0.0.1 
		 */
		static void SetMaxHapticDevices(uint8_t maxHapticDevices);

		/**
		 * @brief Gets the current number of haptic devices that are open.
		 * 
		 * @return @c uint8_t The value of @c sCurrentHapticDevicesCount
		 * 
		 * @since v
		 */
		static uint8_t GetHapticDevicesCount();

		/**
		 * @brief Whether or not the maximum number of haptic devices has been reached.
		 * 
		 * @return @c true if the maximum number of haptic devices has been reached, @c false otherwise.
		 * 
		 * @since v0.0.1
		 */
		static bool HasReachedMaxHapticDevices();

		/**
		 * @brief Opens the haptic device using the @c HappticDeviceID that was provided in the constructor. If the
		 * haptic device is already open, this function will essentially restart the haptic device by closing it and then
		 * opening it again. Keep in mind that this will cause all haptic effects to be lost. Since this class is built
		 * on SDL's haptic feedback API, this function will open the haptic device using SDL's API. Please do note that
		 * if the value of @c sCurrentHapticDevicesCount is equal to the value of @c sMaxHapticDevices, then this function
		 * WILL NOT open the haptic device unless the value of @c sMaxHapticDevices is set to -1.
		 * 
		 * @since v0.0.1
		 */
		void open();

		/**
		 * @brief If the value of @c mbIsOpen is true, then this function will attempt to close the haptic device. otherwise,
		 * this function will do nothing. Additionally, when the haptic device is closed, it will decrement the value of
		 * @c sCurrentHapticDevicesCount.
		 * 
		 * @since v
		 */
		void close();

		void addHapticEffect(std::unique_ptr<IHapticEffect> hapticEffect);

		/**
		 * @brief Checks if the haptic device is open.
		 * 
		 * @return @a true if the haptic device is open, otherwise @a false
		 * 
		 * @since v0.0.1
		 */
		bool isOpen() const;

		/**
		 * @brief Checks if a specific haptic effect can be used on this haptic device.
		 * 
		 * @note Currently, this function does nothing.
		 * 
		 * @param hapticEffect A unique pointer to a haptic effect object instance.
		 * 
		 * @return @a true if the haptic effect can be used on this haptic device, otherwise @a false
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref IHapticEffect
		 */
		bool canUseEffect(std::unique_ptr<IHapticEffect> hapticEffect) const;

		/**
		 * @brief Returns the SDL haptic device pointer that is associated with this haptic device.
		 * 
		 * @return A shared pointer to the SDL haptic device variable in this class.
		 * 
		 * @since v0.0.1
		 */
		std::shared_ptr<SDL_Haptic> getSDLHapticPtr() const;
		
		/**
		 * @brief Gets this device's id.
		 * 
		 * @return The device's id as @c HapticDeviceID
		 * 
		 * @since v
		 */
		HapticDeviceID getHapticDeviceID() const;

		// Operators

		/**
		 * @brief Returns the name of the haptic device as a string. Functionally speaking, this calls the
		 * @code{.cpp} operator const char* () const @endcode and cast it to a string.
		 * 
		 * @return @c std::string The name of the physical device.
		 * 
		 * @since v0.0.1
		 */
		operator std::string() const;

		/**
		 * @brief Returns the name of the haptic device as a string.
		 * 
		 * @return @c const @c char* The name of the physical device.
		 * 
		 * @since v0.0.1
		 */
		operator const char* () const;
	};
}
