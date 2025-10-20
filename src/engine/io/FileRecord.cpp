/**************************************************************************************************
 * @file FileRecord.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <chrono>
#include <memory>
#include <filesystem> 

#include "FileRecord.h"

Atlas::FileRecord::FileRecord(const std::filesystem::path& p)
	: path(p),
	lastUsedMS(std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now().time_since_epoch()).count()) {
}

void Atlas::FileRecord::touch() {
	auto m = std::chrono::duration_cast<std::chrono::milliseconds>(
		steady_clock::now().time_since_epoch()).count();

	lastUsedMS.store(m, std::memory_order_relaxed);
}

Atlas::TimePoint Atlas::FileRecord::getLastUseTime() const {
	auto ms = lastUsedMS.load(std::memory_order_relaxed);

	return TimePoint(std::chrono::milliseconds(ms));
}