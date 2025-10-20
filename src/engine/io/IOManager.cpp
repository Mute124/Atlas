/**************************************************************************************************
 * @file IOManager.cpp
 * 
 * @brief Provides the implementation for the @ref IOManager class, along with it's child classes.
 * 
 * @date October 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
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

#include "../core/Common.h"
#include "../core/Core.h"
#include "../debugging/Logging.h"

#include "IOManager.h"
#include "FileData.h"
#include "FileHandle.h"
#include "FileRecord.h"
#include "IOCommon.h"


// FileJanitor functions:

Atlas::FileManager::FileJanitor::FileJanitor(FileManager& fileManagerRef, const Options& options)
	: mFileManagerRef(fileManagerRef), mOptions(options), mJanitorStopFlag(false)
{
	if (mOptions.bStartJanitor) {
		startJanitor();
	}
}

Atlas::FileManager::FileJanitor::~FileJanitor()
{
	// Stop janitor
	mJanitorStopFlag.store(true);
	mJanitorCV.notify_all();

	// TODO: I dont think this is  needed anymore since the janitor thread is a Jthread

	// Wait for janitor to stop
	if (mJanitorThread.joinable()) {
		mJanitorThread.join();
	}
}

void Atlas::FileManager::FileJanitor::startJanitor() {
	//if (mJanitorThread.joinable()) {
	//	throw std::runtime_error("The janitor thread is already running! Stop it first.");

	//	return;
	//}

	ATLAS_ASSERT(!mJanitorThread.joinable(), "The janitor thread is already running! Stop it first.");

	InfoLog("Starting janitor thread.");

	mJanitorThread = std::jthread([this] { janitorLoop(); });
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
	auto now = steady_clock::now();
	std::vector<std::shared_ptr<FileRecord>> recordChecklist;
	{
		std::shared_lock lock(mFileManagerRef.mMapMutex);
		for (auto const& kv : mFileManagerRef.mRecords) {
			recordChecklist.push_back(kv.second);
		}
	}

	if (recordChecklist.empty()) {
		return;
	}

	for (auto const& rec : recordChecklist) {
		TimePoint last = rec->getLastUseTime();
		auto age = std::chrono::duration_cast<std::chrono::seconds>(now - last);

		if (age >= mOptions.fileTTL) {

			std::unique_lock load_lock(rec->loadMutex);
			if (rec->activeHandles.load(std::memory_order_relaxed) > 0) {

				//InfoLog(std::format("Attempting to evict: {}", rec->path.string()));

				//std::cout << "Attempting to evict: " << rec->path << std::endl;

				if (auto sp = rec->weakDataPtr.lock()) {

					InfoLog(std::format("Evicting file: {}", rec->path.string()));

					//std::cout << "Evicting: " << rec->path << std::endl;
					// safe to unload
					rec->weakDataPtr.reset();
					
					// Check to see if the file was evicted
					if (rec->weakDataPtr.lock() == nullptr) {
						InfoLog(std::format("Evicted file: {}", rec->path.string()));
					}
				}
			} // else skip, it'pathString in use
		}
	}
}

// FileManager functions:

Atlas::FileManager::FileManager(const Options& options)
	: mOptions(options), mJanitor(*this, options)
{
}

Atlas::FileManager::~FileManager()
{
	// No need to deconstruct the janitor, since it will do it automatically

	unloadAll();
}

void Atlas::FileManager::registerDirectory(const std::filesystem::path& dir) {

	// Assertion that checks if the directory exists
	ATLAS_ASSERT(DoesPathExist(dir), std::format("The given path at: {} does not exist! Please make sure that path exists and that you passed the correct path.", dir.string()).c_str());

	//if (!std::filesystem::exists(dir)) {
	//	return;
	//}


	// Recursively iterate through the provided directory, registering each file in the process. 
	// Any file that is found within dir or any of its subdirectories will be registered as well.
	for (auto const& entry : std::filesystem::recursive_directory_iterator(dir)) {
		//InfoLog(std::format("Attempting to register: {}", entry.path().string()));

		if (!entry.is_regular_file()) {
			InfoLog(std::format("Skipping non-file: {}", entry.path().string()));

			//std::cout << "Skipping non-file: " << entry.path().string() << "\n";
			continue;
		}

		auto& entryPathRef = entry.path();

		if (isIgnored(entryPathRef)) {
			InfoLog(std::format("Ignoring: {}", entryPathRef.string()));

			//std::cout << "Ignoring " << entryPathRef.string() << "\n";
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
	auto fileDataSharedPtr = std::make_shared<FileData>(std::move(buf));
	record->weakDataPtr = fileDataSharedPtr;
	record->touch();

	return FileHandle(fileDataSharedPtr, record);
}

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

void Atlas::FileManager::preloadAll() {
	std::vector<std::shared_ptr<FileRecord>> copies;
	{
		std::shared_lock lock(mMapMutex);
		copies.reserve(mRecords.size());

		for (auto const& [path, record] : mRecords) {
			copies.push_back(record);
		}
	}

	for (auto const& r : copies) {
		openFile(r->path);
	}
}

void Atlas::FileManager::unloadAll()
{
	std::shared_lock lock(mMapMutex);

	if (mRecords.empty()) {
		return;
	}

	for (auto const& [path, record] : mRecords) {
		unloadFile(path);
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
