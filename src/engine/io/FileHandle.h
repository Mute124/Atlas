/**************************************************************************************************
 * @file FileHandle.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once
//#include <any>
//#include <string>
//#include <cstdint>
//#include <filesystem>
//#include <iostream>
//#include <fstream>
//#include <iosfwd> // This is just to avoid a transitive include for std::fstream
#include <iosfwd>
#include <memory>

#include "IOCommon.h"
#include "FileRecord.h"
#include "FileData.h"


namespace Atlas {
	using FileStream = std::fstream;

	class FileHandle {
	private:
		std::shared_ptr<FileData> data;
		std::shared_ptr<FileRecord> record;

		void release();
	public:
		FileHandle() = default; // empty handle

		FileHandle(std::shared_ptr<FileData> d, std::shared_ptr<FileRecord> r);
		// moveable
		FileHandle(FileHandle&& o) noexcept;

		FileHandle& operator=(FileHandle&& o) noexcept;

		// not copyable (ensures single logical handle per object)
		FileHandle(const FileHandle&) = delete;
		FileHandle& operator=(const FileHandle&) = delete;

		~FileHandle();

		explicit operator bool() const;

		std::shared_ptr<FileData> get() const;
	};
}
