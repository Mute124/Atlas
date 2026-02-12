/**************************************************************************************************
 * @file VideoDriver.cpp
 * 
 * @brief .
 * 
 * @date February 2026
 * 
 * @since v
 ***************************************************************************************************/
#include <SDL2/SDL_video.h>
#include <stdexcept>

#include "VideoDriver.h"

Atlas::VideoDriver::VideoDriver(std::string const& name, int driverIndex) : cmName(name), cmDriverIndex(driverIndex) {}

Atlas::VideoDriver::VideoDriver(int driverIndex) : cmName(SDL_GetVideoDriver(driverIndex)), cmDriverIndex(driverIndex) {}

inline std::vector<Atlas::VideoDriver> Atlas::VideoDriver::getVideoDrivers() {
	const int cDriverCount = SDL_GetNumVideoDrivers();

	if (cDriverCount <= 0) {
		throw std::runtime_error("No video drivers found or failed to get video driver count");
	}

	std::vector<VideoDriver> drivers;

	for (int i = 0; i < cDriverCount; i++) {
		drivers.emplace_back(SDL_GetVideoDriver(i), i);
	}
}
