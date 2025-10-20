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

#include <string>
#include <any>

namespace Atlas {
	
	enum class EDeviceType {
		Unknown = 0,

	};


	struct DeviceCapabilitiesInfo {
		
	};

	class ADevice {
	protected:

		bool mIsEnabled = false;
		bool mIsReady = false;

		const bool mIsPhysicalDevice = false;
		
		std::string mDeviceName;
		std::string mDeviceVendor;

		int mDeviceGUID;

		std::any mDeviceHandle;
	public:
		
		ADevice() = delete;
		
		explicit ADevice(const bool isPhysicalDevice)
		{
		}
		
		ADevice(const bool isPhysicalDevice, const bool isEnabled, const bool isReady)
		{
		}

		~ADevice() = default;


		virtual void enable()
		{
			this->mIsEnabled = true;
		}
		
		virtual void disable()
		{
			this->mIsEnabled = false;
		}

		virtual void setReady(bool newState)
		{
			this->mIsReady = newState;
		}

		virtual bool isDeviceEnabled()
		{
			return this->mIsEnabled;
		}

		virtual bool isDeviceReady()
		{
			return this->mIsReady;
		}
		
		virtual bool isPhysicalDevice()
		{
			return this->mIsPhysicalDevice;
		}
		
	};
}
