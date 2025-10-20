/**************************************************************************************************
 * @file IOCommon.h
 * 
 * @brief Provides definitions that are used by the IO system.
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <filesystem>

namespace Atlas {
	using PathLocation = std::filesystem::path;
	using steady_clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<steady_clock>;

	bool DoesPathExist(PathLocation const& cPath);

	std::filesystem::path GetAbsolutePath(PathLocation const& cPath);
}
