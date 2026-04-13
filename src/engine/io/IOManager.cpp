/**************************************************************************************************
 * @file IOManager.cpp
 * 
 * @brief Provides the implementation for the @ref IOManager class, along with it's child classes.
 * 
 * @date October 2025
 * 
 * @since v0.0.1
 * 
 *  Copyright 2024 Mute124
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License. 
 * ***************************************************************************************************/
#include <memory>
#include <filesystem>
#include <type_traits>
#include <chrono>
#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>
#include <format>
#include <stdexcept>
#include <algorithm>

#include "../core/Common.h"
#include "../core/Core.h"
#include "../debugging/Logging.h"
#include "../debugging/AException.h"

#include "IOManager.h"
#include "FileData.h"
#include "FileHandle.h"
#include "FileRecord.h"
#include "IOCommon.h"

// FileJanitor functions:

bool Atlas::FileManager::FileJanitor::hasExpired(const std::shared_ptr<FileRecord>& fileRecord) const
{
	const auto cNow = steady_clock::now();
	const TimePoint cLastUseTime = fileRecord->getLastUseTime();
	const auto cFileAge = std::chrono::duration_cast<std::chrono::seconds>(cNow - cLastUseTime);

	return cFileAge >= mOptions.fileTTL;
}

std::vector<std::shared_ptr<Atlas::FileRecord>> Atlas::FileManager::FileJanitor::generateEvictionChecklist()
{
	std::vector<std::shared_ptr<FileRecord>> recordChecklist;

	// Reserving the space now to avoid reallocations during the loop
	recordChecklist.reserve(mFileManagerRef.mRecords.size());

	std::shared_lock lock(mFileManagerRef.mMapMutex);
	for (auto const& [path, record] : mFileManagerRef.mRecords) {
		recordChecklist.push_back(record);
	}

	return recordChecklist;
}

std::string Atlas::FileManager::FileJanitor::tryEvict(const std::shared_ptr<FileRecord>& fileRecord)
{
	std::unique_lock loadLock(fileRecord->loadMutex);

	if (fileRecord->activeHandles.load(std::memory_order_relaxed) > 0) {

		// The file record's path string
		const std::string cRecordFilePath = fileRecord->path.string();

		InfoLog(std::format("Attempting to evict: {}", cRecordFilePath));

		// Checks if the weakDataPtr of a FileRecord object is still valid by calling the lock() method on it
		if (auto recordDataPtr = fileRecord->weakDataPtr.lock()) {

			InfoLog(std::format("Evicting file: {}", cRecordFilePath));

			// safe to unload
			fileRecord->weakDataPtr.reset();

			// Check to see if the file was evicted successfully by seeing if the weakDataPtr is nullptr (this means it was evicted)
			if (fileRecord->weakDataPtr.lock() == nullptr) {
				InfoLog(std::format("Evicted file: {}", cRecordFilePath));
			}
			else {
				ErrorLog(std::format("Failed to evict file: {}", cRecordFilePath));
				return cRecordFilePath;
			}
		}
	} // else skip, it'pathString in use

	return "";
}

std::string Atlas::FileManager::FileJanitor::joinStrings(const std::vector<std::string>& strings, const std::string& separator) const
{
	if (strings.empty()) return "";
	std::ostringstream oss;
	bool first = true;
	for (const auto& str : strings) {
		if (!first) oss << separator;
		oss << str;
		first = false;
	}
	return oss.str();
}

Atlas::FileManager::FileJanitor::FileJanitor(FileManager& fileManagerRef, const Options& options)
	: mFileManagerRef(fileManagerRef), mOptions(options), mJanitorStopFlag(false)
{
	if (mOptions.bStartJanitor) {
		startJanitor();
	}
}

Atlas::FileManager::FileJanitor::~FileJanitor()
{
	stopJanitor();
}

void Atlas::FileManager::FileJanitor::startJanitor() {
	if (mJanitorThread.joinable()) {
		throw AException("The janitor thread is already running! Stop it first.");
	}

	InfoLog("Starting a janitor thread for the file manager.");

	mJanitorThread = std::jthread([this] { janitorLoop(); });
}

void Atlas::FileManager::FileJanitor::stopJanitor()
{
	InfoLog("Stopping the janitor thread for the file manager.");

	// Stop janitor
	mJanitorStopFlag.store(true);
	mJanitorCV.notify_all();


	// Wait for janitor to stop
	if (mJanitorThread.joinable()) {
		InfoLog("Waiting for janitor thread to stop.");
		
		mJanitorThread.join();

		InfoLog("Janitor thread successfully stopped.");
	}
}

void Atlas::FileManager::FileJanitor::janitorLoop()
{
	while (!mJanitorStopFlag.load()) {
		std::unique_lock lock(mJanitorCVMutex);
		mJanitorCV.wait_for(lock, mOptions.evictionCheckInterval);

		if (mJanitorStopFlag.load()) {
			InfoLog("Janitor stop flag set, stopping janitor thread.");
			break;
		}

		evictUnused();
	}
}

void Atlas::FileManager::FileJanitor::evictUnused()
{
	// A list of files to check the eviction status of
	const std::vector<std::shared_ptr<FileRecord>> cRecordChecklist = generateEvictionChecklist();

	// If there is nothing to evict, return as there is nothing to do
	if (cRecordChecklist.empty()) {
		return;
	}
	
	std::vector<std::string> failedEvictions;
	
	std::transform(
		cRecordChecklist.begin(),
		cRecordChecklist.end(),
		std::back_inserter(failedEvictions),
		[&](const std::shared_ptr<FileRecord>& fileRecord) {
			if (hasExpired(fileRecord)) {
				return tryEvict(fileRecord);
			}

			return std::string("");
		}
	);

	// Due to how the system works, remove any strings that are just "". This will get fixed later
	std::erase(failedEvictions, "");

	if (!failedEvictions.empty()) {
		ErrorLog(std::format("Failed to evict {} files. Here are the following files: {}", failedEvictions.size(), joinStrings(failedEvictions, ", ")));
	}
}

bool Atlas::FileManager::FileJanitor::isRunning() const
{
	return mJanitorThread.joinable();
}

bool Atlas::FileManager::FileJanitor::shouldStop() const
{
	return mJanitorStopFlag.load();
}

// FileManager functions:

Atlas::FileManager::FileManager(const Options& options)
	: mOptions(options), mJanitor(*this, options)
{
}

Atlas::FileManager::~FileManager()
{
	shutdown();
}

void Atlas::FileManager::registerDirectory(const std::filesystem::path& dir) {

	// Assertion that checks if the directory exists
	//ATLAS_ASSERT(DoesPathExist(dir), std::format("The given path at: {} does not exist! Please make sure that path exists and that you passed the correct path.", dir.string()).c_str());
	if (!DoesPathExist(dir)) {
		throw AException(std::format("The given path at: {} does not exist! Please make sure that path exists and that you passed the correct path.", dir.string()));
	}
	//if (!std::filesystem::exists(dir)) {
	//	return;
	//}

	// Recursively iterate through the provided directory, registering each file in the process. 
	// Any file that is found within dir or any of its subdirectories will be registered as well.
	for (auto const& entry : std::filesystem::recursive_directory_iterator(dir)) {

		if (!entry.is_regular_file()) {
			InfoLog(std::format("Skipping non-file: {}", entry.path().string()));
			continue;
		}

		auto& entryPathRef = entry.path();

		if (isIgnored(entryPathRef)) {
			InfoLog(std::format("Ignoring: {}", entryPathRef.string()));
			continue;
		}

		// Assuming everything went okay, register the file.
		registerFile(entryPathRef);
	}
}

void Atlas::FileManager::registerFile(const std::filesystem::path& p) {
	auto absolutePath = GetAbsolutePath(p);

	InfoLog(std::format("Registering: {}", absolutePath.string()));

	std::unique_lock lock(mMapMutex);

	auto it = mRecords.find(absolutePath);

	if (it == mRecords.end()) {
		mRecords.try_emplace(absolutePath, std::make_shared<FileRecord>(absolutePath));
		//mRegisteredCount++;
	}
}

// Add ignore regex (pattern uses ECMAScript by default)

void Atlas::FileManager::addIgnorePattern(const std::string& pattern) {
	std::unique_lock lock(mIgnoreMutex);
	mIgnorePatterns.emplace_back(pattern, std::regex::ECMAScript | std::regex::icase);
}

void Atlas::FileManager::removeIgnorePatterns() {
	std::unique_lock lock(mIgnoreMutex);
	mIgnorePatterns.clear();
}

Atlas::FileHandle Atlas::FileManager::openFile(const std::filesystem::path & p) {
	std::filesystem::path absolutePath = GetAbsolutePath(p);
	InfoLog(std::format("Opening file: {}", absolutePath.string()));
	std::shared_ptr<FileRecord> record;
	{
		std::shared_lock mapLock(mMapMutex);
		auto it = mRecords.find(absolutePath);
		
		if (it == mRecords.end()) {
			mapLock.unlock();
			
			registerFile(absolutePath);
			
			mapLock.lock();
			it = mRecords.find(absolutePath);
			
			if (it == mRecords.end())
			{
				ErrorLog(std::format("Failed to open file: {}", absolutePath.string()));
				return {}; // failure
			}
		}
		
		record = it->second;
	}

	record->touch();

	// Fast path: try to get existing loaded data
	if (auto loadedData = record->weakDataPtr.lock()) {
		// return a handle that increments activeHandles
		return FileHandle(loadedData, record);
	}

	std::unique_lock loadLock(record->loadMutex); // A unique lock is required to load the file
	if (auto loadedData = record->weakDataPtr.lock()) {
		return FileHandle(loadedData, record);
	}

	std::shared_ptr<FileData> fileDataSharedPtr;

	std::unique_lock customLoadersLock(mCustomLoadersMutex);
	const std::string extension = absolutePath.extension().string();

	if (mCustomLoaders.contains(extension)) {
		InfoLog(std::format("Using custom loader for the extension: {}", absolutePath.string()));

		FileLoaderFunction loader = mCustomLoaders[extension];

		if (loader) {


			bool result = loader(p, fileDataSharedPtr, record);

			if (!fileDataSharedPtr) {
				ErrorLog(std::format("Custom loader failed to load data into the passed FileDataSharedPtr: {}", absolutePath.string()));
				return {};
			}
			else if (!result) {
				ErrorLog(std::format("Custom loader failed to load data: {}", absolutePath.string()));
				return {};
			}

			//if (!result) {
			//	ErrorLog(std::format("Failed to open file: {}", absolutePath.string()));
			//	return {};
			//}
		}

	}
	else {
		// load from disk
		std::vector<uint8_t> buf;
		std::ifstream ifs(record->path, std::ios::binary | std::ios::ate);

		// Check for failure
		if (!ifs) {
			std::unique_lock mapLock(mMapMutex);

			mRecords.erase(absolutePath);

			// Return a failed handle
			return {};
		}

		// Load the file and read it
		auto size = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		buf.resize(static_cast<size_t>(size)); // Make sure the buffer is big enough
		ifs.read(reinterpret_cast<char*>(buf.data()), size);

		// load data from the disk into memory and store it in the weakDataPtr member of the record object.
		fileDataSharedPtr = std::make_shared<FileData>(std::move(buf));

		ifs.close();
	}

	record->weakDataPtr = fileDataSharedPtr;
	record->touch();

	return FileHandle(fileDataSharedPtr, record);
}

//void Atlas::FileManager::recordFileData(std::shared_ptr<FileData> data, std::shared_ptr<FileRecord> record)
//{
//	record->weakDataPtr = data;
//	record->touch();
//}

bool Atlas::FileManager::unloadFile(const std::filesystem::path& p) {
	auto absolutePath = GetAbsolutePath(p);

	std::shared_lock mapLock(mMapMutex);
	auto it = mRecords.find(absolutePath);

	if (it == mRecords.end()) {
		return false;
	}

	auto& record = it->second;
	std::unique_lock loadLock(record->loadMutex);

	// If there's a pathString loaded data but no active handles, we can unload.
	if (auto sp = record->weakDataPtr.lock()) {
		int activeHandles = record->activeHandles.load(std::memory_order_relaxed);
		
		// Make sure there are no active handles before unloading. if there are active handles,
		// return false because the file is still in use. 
		if (activeHandles == 0) {
			record->weakDataPtr.reset();

			return true;
		}
		else {
			return false;
		}
	}

	return true; // already unloaded
}

size_t Atlas::FileManager::getRegisteredCount() const {
	std::shared_lock lock(mMapMutex);
	return mRecords.size();
}

std::vector<Atlas::FileHandle> Atlas::FileManager::preloadAll() {
	std::vector<std::shared_ptr<FileRecord>> copies;
	{
		std::shared_lock lock(mMapMutex);
		copies.reserve(mRecords.size());

		for (auto const& [path, record] : mRecords) {
			copies.push_back(record);
		}
	}

	std::vector<FileHandle> handles;

	for (auto const& r : copies) {
		handles.push_back(openFile(r->path));

	}

	return handles;
}

void Atlas::FileManager::unloadAll()
{
	std::shared_lock lock(mMapMutex);

	// If the map is empty, nothing to unload, so just return
	if (mRecords.empty()) {
		return;
	}

	// For each record, unload the file
	for (auto const& [path, record] : mRecords) {
		unloadFile(path);
	}
}

void Atlas::FileManager::shutdown()
{
	unloadAll();
	
	if (mJanitor.isRunning()) {
		mJanitor.stopJanitor();
	}
}

std::vector<std::filesystem::path> Atlas::FileManager::findRegistered(const std::regex& re) const {
	std::vector<std::filesystem::path> out;
	std::shared_lock lock(mMapMutex);
	
	for (auto const& [registeredPath, fileRecord] : mRecords) {
		if (std::regex_search(registeredPath.string(), re)) {
			out.push_back(registeredPath);
		}
	}

	return out;
}

bool Atlas::FileManager::isIgnored(const std::filesystem::path& p) const {
	std::lock_guard lock(mIgnoreMutex);
	auto pathString = p.string();

	//return std::ranges::any_of(mIgnorePatterns, 
	//	[&pathString](const std::regex& r) { 
	//		return std::regex_search(pathString, r); 
	//	}
	//);

	// TODO: Replace this for loop with std::ranges::any_of (the above commented out code)
	for (auto const& regexPattern : mIgnorePatterns) {
		// This if statement has to be here because the condition will return the value on the first iteration
		// within the loop. 
		if (std::regex_search(pathString, regexPattern))
		{
			return true;
		}
	}
	
	return false;
}
