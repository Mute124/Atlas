/**************************************************************************************************
 * @file IOManager.cpp
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
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

#include "IOManager.h"


/*
#include "IOManager.h"
#include "PathSearcher.h"
#include "IOCommon.h"

Atlas::IOManager::IOManager(const PathLocation& cExecutablePath, const PathLocation& cCurrentWorkingDirectory) : mExecutablePath(cExecutablePath), mCurrentWorkingDirectory(cCurrentWorkingDirectory) {
}

void Atlas::IOManager::setExecutablePath(const PathLocation& cExecutablePath) {
	mExecutablePath = cExecutablePath; 
}

void Atlas::IOManager::setCurrentWorkingDirectory(const PathLocation& cCurrentWorkingDirectory) { 
	mCurrentWorkingDirectory = cCurrentWorkingDirectory;
}

void Atlas::IOManager::setPathSearcher(std::unique_ptr<APathSearcher> cPathSearcher)
{

	if (cPathSearcher == nullptr) {
		return;
	}
	
	mPathSearcher = std::move(cPathSearcher);
}

void Atlas::IOManager::search(APathSearcher* cPathSearcher)
{
	if (cPathSearcher == nullptr) {
		return;
	}

	cPathSearcher->search();

	mFoundPaths = cPathSearcher->getFoundPaths();
}

void Atlas::IOManager::search()
{
	search(mPathSearcher.get());
}

Atlas::PathLocation Atlas::IOManager::getExecutablePath() const { 
	return mExecutablePath; 
}

Atlas::PathLocation Atlas::IOManager::getCurrentWorkingDirectory() const { 
	return mCurrentWorkingDirectory; 
}

*/



// empty handle




Atlas::FileManager::FileManager(const Options& opts)
	: mOptions(opts), mJanitor(*this, opts)
{
	
	//if (mOptions.bStartJanitor) {
	//	mJanitorThread = std::jthread([this] { janitorLoop(); });
	//}
}

Atlas::FileManager::~FileManager() {
	//// Stop janitor
	//mJanitorStopFlag.store(true);
	//mJanitorCV.notify_all();
	//
	//// I dont think this is needed

	//// Wait for janitor to stop
	//if (mJanitorThread.joinable()) {
	//	mJanitorThread.join();
	//}
}

// Register a directory (recursively); applies ignore regexes.

void Atlas::FileManager::registerDirectory(const std::filesystem::path& dir) {

// For stability reasons, dont assert if directory does not exist on release
#ifdef ATLAS_DEBUG
	// Assertion that checks if the directory exists
	ATLAS_ASSERT(std::filesystem::exists(dir), std::format("The given path at: {} does not exist! Please make sure that path exists and that you passed the correct path.", dir.string()).c_str());
#else
	if (!std::filesystem::exists(dir)) {
		return;
	}
#endif
	for (auto const& entry : std::filesystem::recursive_directory_iterator(dir)) {
		std::cout << "Registering: " << entry.path().string() << "\n";

		if (!entry.is_regular_file()) {
			std::cout << "Skipping non-file: " << entry.path().string() << "\n";
			continue;
		}

		auto& p = entry.path();

		if (isIgnored(p)) {
			std::cout << "Ignoring " << p.string() << "\n";
			continue;
		}

		registerFile(p);
	}
}

// Register single file path (no IO)

void Atlas::FileManager::registerFile(const std::filesystem::path& p) {
	auto abs = std::filesystem::absolute(p);

	std::unique_lock lock(mMapMutex);

	auto it = mRecords.find(abs);

	if (it == mRecords.end()) {
		mRecords.emplace(abs, std::make_shared<FileRecord>(abs));
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

// Open a file: returns shared_ptr<FileData> which keeps file loaded while alive.

Atlas::FileHandle Atlas::FileManager::openFile(const std::filesystem::path & p) {
	std::filesystem::path abs = std::filesystem::absolute(p);
	
	std::shared_ptr<FileRecord> record;
	{
		std::shared_lock map_lock(mMapMutex);
		auto it = mRecords.find(abs);
		
		if (it == mRecords.end()) {
			map_lock.unlock();
			
			registerFile(abs);
			
			map_lock.lock();
			it = mRecords.find(abs);
			
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

	std::unique_lock load_lock(record->loadMutex);
	if (auto loadedData = record->weakDataPtr.lock()) {
		return FileHandle(loadedData, record);
	}

	// load from disk (same as before)
	std::vector<uint8_t> buf;
	std::ifstream ifs(record->path, std::ios::binary | std::ios::ate);
	
	if (!ifs) {
		std::unique_lock map_u(mMapMutex);
		mRecords.erase(abs);
		return {};
	}
	
	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	buf.resize(static_cast<size_t>(size));
	ifs.read(reinterpret_cast<char*>(buf.data()), size);

	auto sp = std::make_shared<FileData>(std::move(buf));
	record->weakDataPtr = sp;
	record->touch();
	return FileHandle(sp, record);
}

// Explicit unload (forces if not in use)

bool Atlas::FileManager::unloadFile(const std::filesystem::path& p) {
	auto abs = std::filesystem::absolute(p);
	std::shared_lock map_lock(mMapMutex);
	auto it = mRecords.find(abs);

	if (it == mRecords.end()) {
		return false;
	}

	auto record = it->second;
	std::unique_lock load_lock(record->loadMutex);

	// If there'pathString loaded data but no active handles, we can unload.
	if (auto sp = record->weakDataPtr.lock()) {
		int active = record->activeHandles.load(std::memory_order_relaxed);
		
		if (active == 0) {
			record->weakDataPtr.reset();

			return true;
		}
		else {
			return false;
		}
	}
	return true; // already unloaded
}

// For debugging / info

size_t Atlas::FileManager::getRegisteredCount() const {
	std::shared_lock lock(mMapMutex);
	return mRecords.size();
}

// Optional: preload all files now (heavy)

void Atlas::FileManager::preloadAll() {
	std::vector<std::shared_ptr<FileRecord>> copies;
	{
		std::shared_lock lock(mMapMutex);
		copies.reserve(mRecords.size());

		for (auto const& kv : mRecords) {
			copies.push_back(kv.second);
		}
	}

	for (auto const& r : copies) {
		openFile(r->path);
	}
}

// find files matching regex in registered map (thread-safe)

std::vector<std::filesystem::path> Atlas::FileManager::findRegistered(const std::regex& re) const {
	std::vector<std::filesystem::path> out;
	std::shared_lock lock(mMapMutex);
	
	for (auto const& kv : mRecords) {
		const auto& p = kv.first;

		if (std::regex_search(p.string(), re)) {
			out.push_back(p);
		}
	}
	return out;
}

bool Atlas::FileManager::isIgnored(const std::filesystem::path& p) const {
	std::lock_guard lock(mIgnoreMutex);
	auto pathString = p.string();
	
	for (auto const& regexPattern : mIgnorePatterns) {
		if (std::regex_search(pathString, regexPattern))
		{
			return true;

		}
	}
	
	return false;
}

//void Atlas::FileManager::janitorLoop() {
//	while (!mJanitorStopFlag.load()) {
//		std::unique_lock lock(mJanitorCVMutex);
//		mJanitorCV.wait_for(lock, mOptions.evictionCheckInterval);
//
//		if (mJanitorStopFlag.load()) {
//			break;
//		}
//
//		evictUnused();
//	}
//}
//
//void Atlas::FileManager::evictUnused() {
//	auto now = steady_clock::now();
//	std::vector<std::shared_ptr<FileRecord>> recordChecklist;
//	{
//		std::shared_lock lock(mMapMutex);
//		for (auto const& kv : mRecords) recordChecklist.push_back(kv.second);
//	}
//
//	if (recordChecklist.empty()) {
//		return;
//	}
//
//	for (auto const& rec : recordChecklist) {
//		TimePoint last = rec->getLastUseTime();
//		auto age = std::chrono::duration_cast<std::chrono::seconds>(now - last);
//
//		if (age >= mOptions.fileTTL) {
//
//			std::unique_lock load_lock(rec->loadMutex);
//			if (rec->activeHandles.load(std::memory_order_relaxed) > 0) {
//
//				std::cout << "Attempting to evict: " << rec->path << std::endl;
//
//				if (auto sp = rec->weakDataPtr.lock()) {
//
//					std::cout << "Evicting: " << rec->path << std::endl;
//					// safe to unload
//					rec->weakDataPtr.reset();
//				}
//			} // else skip, it'pathString in use
//		}
//	}
//}



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

	// I dont think this is needed

	// Wait for janitor to stop
	if (mJanitorThread.joinable()) {
		mJanitorThread.join();
	}
}

void Atlas::FileManager::FileJanitor::startJanitor() {
	mJanitorThread = std::jthread([this] { janitorLoop(); });
}

void Atlas::FileManager::FileJanitor::janitorLoop()
{
	while (!mJanitorStopFlag.load()) {
		std::unique_lock lock(mJanitorCVMutex);
		mJanitorCV.wait_for(lock, mOptions.evictionCheckInterval);

		if (mJanitorStopFlag.load()) {
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

				std::cout << "Attempting to evict: " << rec->path << std::endl;

				if (auto sp = rec->weakDataPtr.lock()) {

					std::cout << "Evicting: " << rec->path << std::endl;
					// safe to unload
					rec->weakDataPtr.reset();
				}
			} // else skip, it'pathString in use
		}
	}
}
//Atlas::FileJanitor::FileJanitor(const Options& options)
//{
//}
//
//void Atlas::FileJanitor::run(FileManager& fileManagerRef)
//{
//	while (!mJanitorStopFlag.load()) {
//		std::unique_lock lock(mJanitorCVMutex);
//		mJanitorCV.wait_for(lock, mOptions.evictionCheckInterval);
//
//		if (mJanitorStopFlag.load()) {
//			break;
//		}
//
//		evictUnused(fileManagerRef);
//	}
//}
//
//void Atlas::FileJanitor::evictUnused(FileManager& fileManagerRef)
//{
//	auto now = steady_clock::now();
//	std::vector<std::shared_ptr<FileRecord>> recordChecklist;
//	{
//		std::shared_lock lock(fileManagerRef.mMapMutex);
//		for (auto const& kv : fileManagerRef.mRecords) recordChecklist.push_back(kv.second);
//	}
//
//	if (recordChecklist.empty()) {
//		return;
//	}
//
//	for (auto const& rec : recordChecklist) {
//		TimePoint last = rec->getLastUseTime();
//		auto age = std::chrono::duration_cast<std::chrono::seconds>(now - last);
//
//		if (age >= mOptions.fileTTL) {
//
//			std::unique_lock load_lock(rec->loadMutex);
//			if (rec->activeHandles.load(std::memory_order_relaxed) > 0) {
//
//				std::cout << "Attempting to evict: " << rec->path << std::endl;
//
//				if (auto loadedData = rec->weakDataPtr.lock()) {
//
//					std::cout << "Evicting: " << rec->path << std::endl;
//					// safe to unload
//					rec->weakDataPtr.reset();
//				}
//			} // else skip, it'pathString in use
//		}
//	}
//}


