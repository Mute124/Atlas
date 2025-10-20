/**************************************************************************************************
 * @file File.cpp
 * 
 * @brief .
 * 
 * @date September 2025
 * 
 * @since v
 ***************************************************************************************************/
#include <any>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <filesystem>
#include <stdexcept>

#include "FileHandle.h"

#include "IOCommon.h"
//
//
//void Atlas::AFile::setFilePath(PathLocation const& cPath) { mFilePath = cPath; }
//
//void Atlas::AFile::setFileSize(size_t cSize) { mFileSize = cSize; }
//
//void Atlas::AFile::setFileType(uint16_t cType) { mFileType = cType; }
//
//void Atlas::AFile::setIsLoaded(bool bIsLoaded) { mIsLoaded = bIsLoaded; }
//
//void Atlas::AFile::setFileData(std::any const& cData) {
//	mFileData = cData;
//}
//
//Atlas::AFile::AFile(PathLocation const& cPath, size_t cSize, uint16_t cType, bool bIsLoaded)
//	: mFilePath(cPath), mFileSize(cSize), mFileType(cType), mIsLoaded(bIsLoaded) {}
//
//Atlas::FileStream Atlas::AFile::open(PathLocation const& cFilePath) {
//	if (!PathExists(cFilePath)) {
//		throw std::runtime_error("File does not exist: " + cFilePath.string());
//	}
//
//	return FileStream(cFilePath);
//}
//
//uint16_t Atlas::AFile::load(PathLocation const& cPath) {
//	mFilePath = cPath;
//
//	std::ifstream file(mFilePath, std::ios::app);
//
//	if (!file.is_open()) {
//		return -1;
//	}
//}
//
//uint16_t Atlas::AFile::append(std::string const& cData) {
//	std::ofstream file(mFilePath, std::ios::app);
//
//	if (!file.is_open() || file.fail()) {
//		return -1;
//	}
//
//	file << cData;
//
//	file.close();
//
//	// Since this is not a finished function, we return -1
//	return -1;
//}
//
//bool Atlas::AFile::isLoaded() const { return mIsLoaded; }
//
//Atlas::PathLocation const& Atlas::AFile::getFilePath() const { return mFilePath; }
//
//size_t Atlas::AFile::getFileSize() const { return mFileSize; }
//
//uint16_t Atlas::AFile::getFileType() const { return mFileType; }
//
//std::any const& Atlas::AFile::getFileData() const { return mFileData; }
//
//Atlas::AFile::operator size_t() const {
//	return getFileSize();
//}
//
//Atlas::AFile::operator bool() const {
//	return isValid();
//}
//
//bool Atlas::PathExists(PathLocation const& cPath)
//{
//	return std::filesystem::exists(cPath);
//}

void Atlas::FileHandle::release() {
	if (record) {
		record->activeHandles.fetch_sub(1, std::memory_order_relaxed);
		record.reset();
	}

	data.reset();
}

Atlas::FileHandle::FileHandle(std::shared_ptr<FileData> d, std::shared_ptr<FileRecord> r)
	: data(std::move(d)), record(std::move(r))
{
	if (record) {
		record->activeHandles.fetch_add(1, std::memory_order_relaxed);
	}
}

// moveable

Atlas::FileHandle::FileHandle(FileHandle&& o) noexcept
	: data(std::move(o.data)), record(std::move(o.record)) {
}

Atlas::FileHandle& Atlas::FileHandle::operator=(FileHandle&& o) noexcept {
	if (this == &o)
	{
		return *this;
	}

	release();

	data = std::move(o.data);
	record = std::move(o.record);

	return *this;
}

Atlas::FileHandle::~FileHandle() {
	release();
}

Atlas::FileHandle::operator bool() const {
	return static_cast<bool>(data);
}

std::shared_ptr<Atlas::FileData> Atlas::FileHandle::get() const {
	return data;
}