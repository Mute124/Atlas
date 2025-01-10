#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <any>
#include <functional>

#include "../utils/Singleton.h"
#include "libconfig.h++"
#include "../dbg/Logging.h"

namespace Atlas {
	using DirectoryFiles = std::vector<std::string>;

	class FileMeta {
	public:
		// IMPORTANT: Searching by extension is highly discouraged because there can be multiple files with the same extension. Only use this if you know what you are doing.
		std::string extension;
		std::string path;
		std::string filename;
	};

	class RegisteredFile {
	public:
		bool isLoaded = false;
		std::any data = nullptr;
		std::shared_ptr<FileMeta> meta = nullptr;
		uint16_t index = 0;
		std::mutex loadMutex = std::mutex();
		
		RegisteredFile() {}
		explicit RegisteredFile(std::shared_ptr<FileMeta> const& meta) : meta(meta) {}

		template<typename T>
		T get() { return std::any_cast<T>(data); }
	};

	bool FileSystemIsProperlyFormatted(const char* rootPath, const char* directoryMapPath);

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
		std::shared_ptr<RegisteredFile> getFile(std::string key, bool loadIfNotLoaded = true) {
			uint16_t index = this->mLookupTable[key];
			std::shared_ptr<RegisteredFile> file = this->mRegisteredFiles[index];

			if (loadIfNotLoaded && !file->isLoaded) this->loadFile(file);

			return file;
		}
		
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

	FileSystemRegistry& GetFileSystemRegistry();
	void InitializeFileRegistry(const char* rootPath);
	void AddFileRegistryLoadFunction(std::string extension, std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc);
	void AddFileRegistryLoadOnRegisterExtension(std::string const& extension);

	std::shared_ptr<FileMeta> GetFileMeta(std::string key);
	std::shared_ptr<RegisteredFile> GetFile(std::string key, bool loadIfNotLoaded = true);
}
