#pragma once

#include <any>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "FileMeta.h"
#include "RegisteredFile.h"

#include "../dbg/ELogLevel.h"
#include "../dbg/Logging.h"
#include "../utils/Singleton.h"

namespace Atlas {
	class FileSystemRegistry : public Singleton<FileSystemRegistry> {
	public:

		FileSystemRegistry() {}

		/// <summary>
		/// Initializes the specified root path.
		/// </summary>
		/// <param name="rootPath">The root path.</param>
		void init(const char* rootPath);

		/// <summary>
		/// Gets the file meta.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <returns></returns>
		std::shared_ptr<FileMeta> GetFileMeta(std::string const& key);

		/// <summary>
		/// Loads the file.
		/// </summary>
		/// <param name="file">The file.</param>
		void loadFile(std::shared_ptr<RegisteredFile> file) {
			std::scoped_lock lock(file->loadMutex);
			Log("Loading file: " + file->meta->filename + " (" + file->meta->extension + ")", ELogLevel::TRACE);
			std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc = mLoadingFunctions.at(file->meta->extension);

			file->data = loadFunc(file->meta);

			file->isLoaded = true;
		}

		/// <summary>
		/// Gets the file.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="loadIfNotLoaded">if set to <c>true</c> [load if not loaded].</param>
		/// <returns></returns>
		std::shared_ptr<RegisteredFile> getFile(std::string key, bool loadIfNotLoaded = true);

		/// <summary>
		/// Adds the load function.
		/// </summary>
		/// <param name="extension">The extension.</param>
		/// <param name="loadFunc">The load function.</param>
		void addLoadFunction(std::string extension, std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc) { this->mLoadingFunctions[extension] = loadFunc; }

		/// <summary>
		/// Adds the load on register extension.
		/// </summary>
		/// <param name="extension">The extension.</param>
		void addLoadOnRegisterExtension(std::string extension) { this->mLoadOnRegisterExtensionsSet.insert(extension); }

	private:
		static inline uint16_t sFileIndex = 0;
		static inline uint16_t GetNextFileIndex() { return sFileIndex++; }

		// Holds the functions that will be called when a file is loaded
		std::unordered_map<std::string, std::function<std::any(std::shared_ptr<FileMeta>)>> mLoadingFunctions;

		// Tells the registry which extensions should be loaded on register. If it exists in the set, it will be loaded
		std::unordered_set<std::string> mLoadOnRegisterExtensionsSet;

		std::unordered_map<uint16_t, std::shared_ptr<RegisteredFile>> mRegisteredFiles;
		std::unordered_map<uint16_t, std::shared_ptr<FileMeta>> mFileMetaRegistry;
		std::unordered_map<std::string, uint16_t> mLookupTable;

		friend class IProject;
	};
}