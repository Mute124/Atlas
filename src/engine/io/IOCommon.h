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

	bool DoesPathExist(PathLocation const& cPath);
}
