/**************************************************************************************************
 * @file VideoDriver.h
 * 
 * @brief .
 * 
 * @date February 2026
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <string>
#include <vector>

#include "../../core/Core.h"

namespace Atlas {
	class VideoDriver {
	private:
		const std::string cmName;
		const int cmDriverIndex;

	public:
		ATLAS_EXPLICIT VideoDriver(std::string const& name, int driverIndex);
		ATLAS_EXPLICIT VideoDriver(int driverIndex);

		static std::vector<VideoDriver> getVideoDrivers();
	};
}
