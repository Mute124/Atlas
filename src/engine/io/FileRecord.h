/**************************************************************************************************
 * @file FileRecord.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <filesystem>
#include <atomic>
#include <memory>
#include <mutex>

#include "IOCommon.h"
#include "FileData.h"

namespace Atlas {
	struct FileRecord {
		std::filesystem::path path;
		std::atomic<TimePoint::rep> lastUsedMS; // store as integral ms to be atomic
		std::mutex loadMutex; // protects loading/unloading for this record
		std::weak_ptr<FileData> weakDataPtr; // points to loaded data if loaded
		std::atomic<int> activeHandles{ 0 }; // NEW: number of FileHandles currently held

		explicit FileRecord(const std::filesystem::path& p);

		void touch();

		TimePoint getLastUseTime() const;
	};
}
