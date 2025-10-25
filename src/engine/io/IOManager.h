/**************************************************************************************************
 * @file IOManager.h
 * 
 * @brief Contains the definitions required for managing files. 
 * 
 * @details Below is a clarification of terms that are used in this file:
 * - Eviction: The process of removing a file after a certain amount of time has elapsed since it was
 * last used. Usually, this is done by the @c FileJanitor.
 * 
 * @date October 2025
 * 
 * @since v0.0.1
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

#include "FileData.h"
#include "FileRecord.h"
#include "FileHandle.h"

/**
 * @brief The default time-to-live value for a file.
 * 
 * @since v0.0.1
 */
#define ATLAS_DEFAULT_FILE_TTL 60

/**
 * @brief The default interval at which the janitor will check for unused files.
 * 
 * @since v0.0.1
 */
#define ATLAS_DEFAULT_JANITOR_CHECK_INTERVAL 10

/**
 * @brief The default value for whether or not to start the janitor.
 * 
 * @since v0.0.1
 */
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

	/**
	 * @brief Some information about Atlas' io module that may be helpful for debugging
	 * 
	 * @since v0.0.1
	 */
	struct FileSystemAuditInfo {
		size_t totalRegisteredEntries{ 0 };
		size_t totalLoadedEntries{ 0 };
	};

	/**
	 * @brief As the name suggests, this @b non-copyable class is responsible for managing the files that Atlas and the
	 * user has loaded. This class has @b 3 main responsibilities (excluding the janitor): @b Loading, @b Unloading and
	 * @b Registering. Additionally, this class can be configured to ignore certain patterns it comes across during the
	 * file discovery process through the use of @b regex patterns. 
	 * 
	 * @since v0.0.1
	 */
    class FileManager {
	public:

		/**
		 * @brief Configuration options for the @c FileManager and its @c FileJanitor.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref FileJanitor
		 * 
		 * @sa @ref FileManager
		 */
		struct Options {
			/**
			 * @brief How often should the @c FileJanitor should check for inactive files in @b seconds. This is set
			 * to @c ATLAS_DEFAULT_JANITOR_CHECK_INTERVAL by default. Please refer to the note below for an important
			 * piece of advice regarding what to set this value to.
			 * 
			 * @note As a piece of advice, this should be a multiple of @c fileTTL to ensure that the file is not staying
			 * loaded for longer than @c fileTTL. For example, if @c fileTTL is 10 seconds, then @c evictionCheckInterval
			 * should be set to 1, 2, 5 or 10 seconds. Once the file is inactive for 10 seconds, it will be unloaded
			 * right after the file's lifespan is over, because @c evictionCheckInterval is 10 seconds.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref ATLAS_DEFAULT_JANITOR_CHECK_INTERVAL
			 * @sa @ref FileJanitor
			 * @sa @ref fileTTL
			 */
			std::chrono::seconds evictionCheckInterval{ ATLAS_DEFAULT_JANITOR_CHECK_INTERVAL }; // janitor check period

			/**
			 * @brief How long should the @c FileJanitor wait, in seconds, for a file to be used before unloading it. This
			 * is set to @c ATLAS_DEFAULT_FILE_TTL by default.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref ATLAS_DEFAULT_FILE_TTL
			 * @sa @ref FileJanitor
			 * @sa @ref evictionCheckInterval
			 * 
			 * @todo Perhaps consider moving this so that each file has its own ttl? If not, maybe there should be some
			 * kind of system for each file type/extension to have its own ttl. This would make it much easier to
			 * customize how the janitor reacts to each file.
			 */
			std::chrono::seconds fileTTL{ ATLAS_DEFAULT_FILE_TTL };
		
			/**
			 * @brief The maximum number of files that can be loaded at once. This is set to @c -1 by default, which
			 * indicates there is no limit. If you want to set a limit, set this value to a positive number.
			 * 
			 * @note If this limit is reached, the janitor will unload the least recently used file. This should be
			 * set to -1 for most cases, unless there is a specific reason to limit the number of files. This number
			 * should be set to a positive number if you want to limit the number of files.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref ATLAS_DEFAULT_MAX_FILE_COUNT
			 * @sa @ref FileJanitor
			 */
			size_t maxFileCount{ (size_t)(-1)}; // -1 = no limit
			
			/**
			 * @brief Whether or not the janitor should start automatically.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref ATLAS_DEFAULT_START_JANITOR
			 * @sa @ref FileJanitor
			 */
			bool bStartJanitor{ ATLAS_DEFAULT_START_JANITOR };
		};

		/**
		 * @brief Automatically evicts unused files in a background thread and cleans up the @c mRecords map in the @c FileManager. 
		 * Furthermore, this class also handles the janitor thread, which is where the janitor loop runs. This class is handled by
		 * the @c FileManager class. It is not recommended to use this class directly, and let the @c FileManager handle it. However,
		 * if you do infact need to use this class directly, please refer to the details section in the documentation below.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref FileManager
		 */
		class FileJanitor {
		private:

			/**
			 * @brief A reference to the @c FileManager class.
			 * 
			 * @since v0.0.1
			 */
			FileManager& mFileManagerRef;

			/**
			 * @brief The configuration options for the janitor. This should be the same as the one passed to the @c FileManager.
			 * 
			 * @since v0.0.1
			 */
			Options mOptions;

			/**
			 * @brief An atomic flag that indicates whether or not the janitor should stop.
			 * 
			 * @since v0.0.1
			 */
			std::atomic<bool> mJanitorStopFlag;
			
			/**
			 * @brief A condition variable that is used to notify the janitor thread that it should check for unused files.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref mJanitorCVMutex 
			 */
			std::condition_variable_any mJanitorCV;

			/**
			 * @brief Syncronizes access to @c mJanitorCV
			 * 
			 * @since v0.0.1
			 */
			std::mutex mJanitorCVMutex;

			/**
			 * @brief The thread that the janitor will run on.
			 * 
			 * @since v0.0.1
			 */
			std::jthread mJanitorThread;

		public:

			ATLAS_DISALLOW_COPY(FileJanitor);

			/**
			 * @brief Initializes a new instance of the @c FileJanitor class.
			 * 
			 * @param fileManagerRef A reference to an instance of the @c FileManager class.
			 * @param options The configuration options for the janitor. This is the same as the one passed to the @c FileManager
			 * in it's constructor.
			 * 
			 * @since v0.0.1
			 */
			FileJanitor(FileManager& fileManagerRef, const Options& options);

			FileJanitor() = delete;
			
			/**
			 * @brief Stops the janitor thread and waits for it to stop.
			 * 
			 * @since v0.0.1
			 */
			~FileJanitor();

			/**
			 * @brief Starts the janitor thread if it is not already running.
			 * 
			 * @warning This will assert if the janitor thread is already running.
			 * 
			 * @since v0.0.1
			 */
			void startJanitor();

			/**
			 * @brief Runs the janitor loop, which checks for inactive files and evicts them if they have been inactive for too long.
			 * 
			 * @note Under normal circumstances, this runs on the janitor thread.
			 * 
			 * @since v0.0.1
			 */
			void janitorLoop();

			/**
			 * @brief Checks for inactive files and evicts them if they have been inactive for too long, assuming nothing is currently
			 * using the file.
			 * 
			 * @since v0.0.1
			 */
			void evictUnused();

			bool isRunning() const;

			bool shouldStop() const;
		};
	private:
		friend class FileJanitor;

		/**
		 * @brief The janitor for this class.
		 * 
		 * @since v0.0.1
		 */
		FileJanitor mJanitor;

		/**
		 * @brief A shared mutex that is used to guard access to the @c mRecords map.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref mRecords
		 */
		mutable std::shared_mutex mMapMutex;

		/**
		 * @brief A map of registered files. This is guarded by @c mMapMutex.
		 * 
		 * @since v0.0.1
		 */
		std::unordered_map<std::filesystem::path, std::shared_ptr<FileRecord>> mRecords; 
		
		/**
		 * @brief This class' configuration options.
		 * 
		 * @since v0.0.1
		 */
		Options mOptions;

		// ignore patterns
		
		/**
		 * @brief A mutex that is used to guard access to the @c mIgnorePatterns vector.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref mIgnorePatterns
		 */
		mutable std::mutex mIgnoreMutex;

		/**
		 * @brief A vector of regex ignore patterns. This is guarded by @c mIgnoreMutex.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref mIgnoreMutex
		 */
		std::vector<std::regex> mIgnorePatterns;

		/**
		 * @brief Checks if a path should be ignored based on the ignore patterns.
		 * 
		 * @param p
		 * 
		 * @return 
		 * 
		 * @since v
		 */
		bool isIgnored(const std::filesystem::path& p) const;

    public:
		// Disallow copy
		ATLAS_DISALLOW_COPY(FileManager);

		/**
		 * @brief Initializes a new instance of the @c FileManager class with the specified configuration.
		 * 
		 * @param options The configuration options for the file manager.
		 * 
		 * @since v0.0.1
		 */
        explicit FileManager(const Options& options = Options());

		/**
		 * @brief Destroys the current instance of the @c FileManager class.
		 * 
		 * @since v0.0.1
		 */
        ~FileManager();

		/**
		 * @brief Register a directory (recursively); applies ignore regexes.
		 * 
		 * @param dir The directory to register.
		 * 
		 * @since v0.0.1
		 */
        void registerDirectory(const std::filesystem::path& dir);

		/**
		 * @brief Register a file; applies ignore regexes. The file must exist.
		 * 
		 * @param p The file to register.
		 * 
		 * @since v0.0.1
		 */
        void registerFile(const std::filesystem::path& p);

        // Add ignore regex (pattern uses ECMAScript by default)

		/**
		 * @brief Add an ignore pattern to the ignore list using ECMAScript regex. Files matching this pattern will not be loaded.
		 * 
		 * @param pattern A ECMAScript regex pattern.
		 * 
		 * @since v0.0.1
		 */
		void addIgnorePattern(const std::string& pattern);

		/**
		 * @brief Remove all ignore patterns.
		 * 
		 * @since v0.0.1
		 */
        void removeIgnorePatterns();

		/**
		 * @brief Open a file.
		 * 
		 * @param p The path to the file that should be opened.
		 * 
		 * @warning Do not discard the return value! It keeps the file loaded while alive.
		 * 
		 * @return @c shared_ptr<FileData> This return value keeps the file loaded while alive.
		 * 
		 * @since v0.0.1
		 */
		FileHandle openFile(const std::filesystem::path& p);

		/**
		 * @brief An explicit unload function that unloads a file (forces if not in use).
		 * 
		 * @param p The path to the file that should be unloaded.
		 * 
		 * @return @c bool Whether the file was unloaded or not.
		 * 
		 * @since v0.0.1
		 */
        bool unloadFile(const std::filesystem::path& p);

        /**
         * @brief Get the number of registered files. This can be used for debugging purposes.
         * 
         * @return @c size_t The number of registered files.
         * 
         * @since v0.0.1
         */
        size_t getRegisteredCount() const;

        /**
         * @brief Preloads all registered files. This is an optional function, and it is extremely expensive.
         * 
         * @note As an edge case, this could be used to measure loading times.
         * 
         * @since v0.0.1
         */
        void preloadAll();

		/**
		 * @brief Unloads all files.
		 * 
		 * @since v0.0.1
		 */
		void unloadAll();

        /**
         * @brief Finds all registered files that match the specified regex pattern.
         * 
         * @param re An ECMAScript regex pattern to use for searching.
         * 
         * @return @c std::vector<std::filesystem::path> A vector of paths that match the pattern.
         * 
         * @since v0.0.1
         */
        std::vector<std::filesystem::path> findRegistered(const std::regex& re) const;
    };
}
