/**************************************************************************************************
 * @file IOManager.h
 * 
 * @brief .
 * 
 * @date October 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <filesystem>
#include <unordered_map>
#include <memory>
#include <vector>
#include <any>
#include <string>
#include <cstdint>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <regex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


#include "../core/Core.h"
#include "../core/Common.h"
#include "IOCommon.h"
#include "PathSearcher.h"

#include "FileData.h"
#include "FileRecord.h"
#include "FileHandle.h"

#define ATLAS_DEFAULT_FILE_TTL 60
#define ATLAS_DEFAULT_JANITOR_CHECK_INTERVAL 10
#define ATLAS_DEFAULT_START_JANITOR true

namespace Atlas {
	

	/*
	// This is like a filter for various options that may happen during the file discovery process
	class IPathLoaderLogicGate {
	public:

		IPathLoaderLogicGate() = default;

		virtual ~IPathLoaderLogicGate() = default;

		virtual bool shouldLoadOnDiscovery(PathLocation const& cPath) = 0;
		
		virtual bool shouldLoadPath(PathLocation const& cPath) = 0;
	};

	class PathLoaderLogicGate : public IPathLoaderLogicGate {
	private:

	public:

		bool shouldLoadOnDiscovery(PathLocation const& cPath) override { return true; }

		bool shouldLoadPath(PathLocation const& cPath) override { return true; }
	};

	class AExtensionValidator {
	private:

	public:
	
		bool isValid(PathLocation const& cPath) const { return true; }
	};

	class IOManager {
	private:
		
		struct ExtensionHandlersAggregator {
			std::unique_ptr<AExtensionValidator> extensionValidator;
			//std::unique_ptr<AParser> parser;
		};

		std::unordered_map<std::string, ExtensionHandlersAggregator> mExtensionHandlers;

		PathLocation mExecutablePath;
		PathLocation mCurrentWorkingDirectory;

		std::unique_ptr<APathSearcher> mPathSearcher;

		std::vector<FoundPathResult> mFoundPaths;
	public:

		IOManager() = default;

		explicit IOManager(const PathLocation& cExecutablePath, const PathLocation& cCurrentWorkingDirectory);

		void setExecutablePath(const PathLocation& cExecutablePath);

		void setCurrentWorkingDirectory(const PathLocation& cCurrentWorkingDirectory);
	
		void setPathSearcher(std::unique_ptr<APathSearcher> cPathSearcher);

		void search(APathSearcher* cPathSearcher);

		void search();



		PathLocation getExecutablePath() const;

		PathLocation getCurrentWorkingDirectory() const;
	};*/



	



	//class FileJanitor {
	//public:
	//	struct Options {
	//		std::chrono::seconds evictionCheckInterval{ 10 }; // janitor check period
	//		std::chrono::seconds fileTTL{ 60 };                // unload files not used for this TTL
	//		bool bStartJanitor{ true };
	//	};

	//private:
	//	Options mOptions;

	//	std::atomic<bool> mJanitorStopFlag;
	//	std::condition_variable_any mJanitorCV;
	//	std::mutex mJanitorCVMutex;
	//	std::thread mJanitorThread;

	//public:

	//	FileJanitor(const Options& options);

	//	void run(FileManager& fileManagerRef);

	//	void evictUnused(FileManager& fileManagerRef);

	//};

	/**
	 * @brief Some information about Atlas' io module that may be helpful for debugging
	 * 
	 * @since v0.0.1
	 */
	struct FileSystemAuditInfo {
		size_t totalRegisteredEntries{ 0 };
		size_t totalLoadedEntries{ 0 };
	};

    class FileManager {
	public:
		struct Options {
			std::chrono::seconds evictionCheckInterval{ ATLAS_DEFAULT_JANITOR_CHECK_INTERVAL }; // janitor check period
			std::chrono::seconds fileTTL{ ATLAS_DEFAULT_FILE_TTL };                // unload files not used for this TTL
		
			size_t maxFileCount{ (size_t)(-1)}; // -1 = no limit
			
			bool bStartJanitor{ ATLAS_DEFAULT_START_JANITOR };
		};

		class FileJanitor {
		private:
			FileManager& mFileManagerRef;

			Options mOptions;

			// janitor thread
			std::atomic<bool> mJanitorStopFlag;
			std::condition_variable_any mJanitorCV;
			std::mutex mJanitorCVMutex;
			std::jthread mJanitorThread;

		public:

			FileJanitor(FileManager& fileManagerRef, const Options& options);

			FileJanitor() = delete;

			~FileJanitor();

			void startJanitor();

			void janitorLoop();

			void evictUnused();
		};
	private:
		friend class FileJanitor;

		mutable std::shared_mutex mMapMutex;
		std::unordered_map<std::filesystem::path, std::shared_ptr<FileRecord>> mRecords; // guarded by mMapMutex
		Options mOptions;
		//Counter mRegisteredFilesCounter;


		// ignore patterns
		mutable std::mutex mIgnoreMutex;
		std::vector<std::regex> mIgnorePatterns;

		bool isIgnored(const std::filesystem::path& p) const;

		FileJanitor mJanitor;

		//void janitorLoop();

		//void evictUnused();

    public:

        explicit FileManager(const Options& opts = Options());

        ~FileManager();

        // Disallow copy
        FileManager(const FileManager&) = delete;
        FileManager& operator=(const FileManager&) = delete;

        // Register a directory (recursively); applies ignore regexes.
        void registerDirectory(const std::filesystem::path& dir);

        // Register single file path (no IO)
        void registerFile(const std::filesystem::path& p);

        // Add ignore regex (pattern uses ECMAScript by default)
        void addIgnorePattern(const std::string& pattern);

        void removeIgnorePatterns();

        // Open a file: returns shared_ptr<FileData> which keeps file loaded while alive.
		FileHandle openFile(const std::filesystem::path& p);

        // Explicit unload (forces if not in use)
        bool unloadFile(const std::filesystem::path& p);

        // For debugging / info
        size_t getRegisteredCount() const;

        // Optional: preload all files now (heavy)
        void preloadAll();

        // find files matching regex in registered map (thread-safe)
        std::vector<std::filesystem::path> findRegistered(const std::regex& re) const;
    };
}
