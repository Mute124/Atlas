/**************************************************************************************************
 * @file IOCommon.cpp
 * 
 * @brief Definitions for @ref IOCommon.h.
 * 
 * @date October 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#include <filesystem>

#include "IOCommon.h"

bool Atlas::DoesPathExist(PathLocation const& cPath)
{
    return std::filesystem::exists(cPath);
}
