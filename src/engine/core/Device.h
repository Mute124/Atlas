/**************************************************************************************************
 * @file Device.h
 * 
 * @brief Declaration code for using devices.
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

namespace Atlas {
	
	class ADevice {
	private:

		bool mIsEnabled = false;

		bool mIsReady = false;

		const bool mIsPhysicalDevice = false;

	public:
		
		ADevice() = delete;
		
		explicit ADevice(const bool isPhysicalDevice);
		
		ADevice(const bool isPhysicalDevice, const bool isEnabled, const bool isReady);

		~ADevice() = default;

		virtual void init() = 0;

		virtual void open() = 0;

		virtual void close() = 0;

		virtual void enable();
		
		virtual void disable();

		virtual void setReady(bool newState);

		virtual bool isDeviceEnabled();

		virtual bool isDeviceReady();
		
		virtual bool isPhysicalDevice();
		
	};
}
