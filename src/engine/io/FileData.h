/**************************************************************************************************
 * @file FileData.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
#include <vector>
#include <cstdint>

namespace Atlas {
	struct FileData {
		std::vector<uint8_t> bytes;

		explicit FileData(std::vector<uint8_t>&& b);
	};
}
