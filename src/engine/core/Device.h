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
#include <cstdint>
#include <any>
#include <thread>

#include "Core.h"

namespace Atlas {
	
	enum class EDeviceType {
		Unknown = 0,

	};

	struct DeviceCapabilitiesInfo {
		
	};

	class IDeviceInfo {
	private:
		//const bool mIsPhysicalDevice = false;

	//	std::string mDeviceName;
	//	std::string mDeviceVendor;

	//	//int mDeviceGUID;

	//protected:
	//	void setDeviceName(const std::string_view name) { 
	//		mDeviceName = name;
	//	}

	//	void setDeviceVendor(const std::string_view vendor) { 
	//		mDeviceVendor = vendor;
	//	}

	//	explicit ADeviceInfo(const std::string_view name, const std::string_view vendor) {
	//		setDeviceName(name);
	//	}

	public:

		IDeviceInfo() = default;
		virtual ~IDeviceInfo() = default;

		virtual void enumerate() = 0;

		//std::string_view getDeviceName() const { 
		//	return mDeviceName;
		//}
	};

	class CPUDeviceInfo {
	private:


		int mL1CacheLineSize;
		int mLogicalCoreCount;
		uint32_t mHardwareConcurrency;
	public:

		//CPUDeviceInfo(const int logicalCoreCount, const uint32_t hardwareConcurrency);
		//ATLAS_EXPLICIT CPUDeviceInfo(const int logicalCoreCount);
		CPUDeviceInfo();

		static inline int GetL1CacheLineSize();
		static inline int GetLogicalCoreCount();
		static inline uint32_t HardwareConcurrency();

		//void enumerate() override;
		
		int getL1CacheLineSize() const;
		int getLogicalCoreCount() const;
		uint32_t getHardwareConcurrency() const;
	};
}
