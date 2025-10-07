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

#include "File.h"

#include "IOCommon.h"


void Atlas::AFile::setFilePath(PathLocation const& cPath) { mFilePath = cPath; }

void Atlas::AFile::setFileSize(size_t cSize) { mFileSize = cSize; }

void Atlas::AFile::setFileType(uint16_t cType) { mFileType = cType; }

void Atlas::AFile::setIsLoaded(bool bIsLoaded) { mIsLoaded = bIsLoaded; }

void Atlas::AFile::setFileData(std::any const& cData) {
	mFileData = cData;
}

Atlas::AFile::AFile(PathLocation const& cPath, size_t cSize, uint16_t cType, bool bIsLoaded)
	: mFilePath(cPath), mFileSize(cSize), mFileType(cType), mIsLoaded(bIsLoaded) {}

Atlas::FileStream Atlas::AFile::open(PathLocation const& cFilePath) {
	if (!PathExists(cFilePath)) {
		throw std::runtime_error("File does not exist: " + cFilePath.string());
	}

	return FileStream(cFilePath);
}

uint16_t Atlas::AFile::load(PathLocation const& cPath) {
	mFilePath = cPath;

	std::ifstream file(mFilePath, std::ios::app);

	if (!file.is_open()) {
		return -1;
	}
}

uint16_t Atlas::AFile::append(std::string const& cData) {
	std::ofstream file(mFilePath, std::ios::app);

	if (!file.is_open() || file.fail()) {
		return -1;
	}

	file << cData;

	file.close();

	// Since this is not a finished function, we return -1
	return -1;
}

bool Atlas::AFile::isLoaded() const { return mIsLoaded; }

Atlas::PathLocation const& Atlas::AFile::getFilePath() const { return mFilePath; }

size_t Atlas::AFile::getFileSize() const { return mFileSize; }

uint16_t Atlas::AFile::getFileType() const { return mFileType; }

std::any const& Atlas::AFile::getFileData() const { return mFileData; }

Atlas::AFile::operator size_t() const {
	return getFileSize();
}

Atlas::AFile::operator bool() const {
	return isValid();
}

bool Atlas::PathExists(PathLocation const& cPath)
{
	return std::filesystem::exists(cPath);
}
