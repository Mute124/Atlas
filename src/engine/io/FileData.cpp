/**************************************************************************************************
 * @file FileData.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <vector>
#include <cstdint>
#include <type_traits>

#include "FileData.h"

Atlas::FileData::FileData(std::vector<uint8_t>&& b)
	: bytes(std::move(b)) {
}