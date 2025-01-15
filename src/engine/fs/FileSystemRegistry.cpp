#include "../dbg/ELogLevel.h"
#include "../dbg/Logging.h"
#include "FileMeta.h"
#include "FileSystemRegistry.h"
#include "RegisteredFile.h"
#include "RegisteredFile.h"

#include <raylib.h>

#include <cstdint>
#include <format>
#include <memory>
#include <string>

void Atlas::FileSystemRegistry::init(const char* rootPath)
{
	Logger::Instance().log("Initializing File Registry...");

	Log("Scanning " + std::string(rootPath) + " for files...");
	FilePathList files = LoadDirectoryFilesEx(rootPath, nullptr, true);
	Log("Found " + std::to_string(files.count) + " files.");


	for (int i = 0; i < files.count; i++) {
		std::string path = files.paths[i];
		std::string filename = path.substr(path.find_last_of("\\") + 1);
		std::string extension = filename.substr(filename.find_last_of(".") + 1);

		// create file meta
		FileMeta meta;
		meta.path = path;
		meta.filename = filename;
		meta.extension = extension;

		// Get file index
		uint16_t index = GetNextFileIndex();

		// add to configLookup table
		this->mLookupTable[meta.path] = index;
		this->mLookupTable[meta.filename] = index;
		this->mLookupTable[meta.extension] = index;

		// Register file
		this->mFileMetaRegistry[index] = std::make_shared<FileMeta>(meta);

		this->mRegisteredFiles[index] = std::make_shared<RegisteredFile>(std::make_shared<FileMeta>(meta));

		if (this->mLoadOnRegisterExtensionsSet.contains(extension)) {
			loadFile(this->mRegisteredFiles[index]);
		}

		std::string output = "";

		output = std::format("File with index[{}], named: {} with an extension of {} was registered", index, filename, path, extension);
		Log(output, ELogLevel::TRACE);
	}
}

std::shared_ptr<Atlas::FileMeta> Atlas::FileSystemRegistry::GetFileMeta(std::string const& key)
{
	uint16_t index = this->mLookupTable[key];

	return this->mFileMetaRegistry[index];
}

/// <summary>
/// Loads the file.
/// </summary>
/// <param name="file">The file.</param>

void Atlas::FileSystemRegistry::loadFile(std::shared_ptr<RegisteredFile> file) {
	std::scoped_lock lock(file->getLoadMutex());
	std::shared_ptr<FileMeta> fileMeta = file->getFileMeta();

	Log("Loading file: " + fileMeta->filename + " (" + fileMeta->extension + ")", ELogLevel::TRACE);
	std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc = mLoadingFunctions.at(fileMeta->extension);

	file->mFileData = loadFunc(fileMeta);

	file->mIsLoaded = true;
}

/// <summary>
/// Gets the file.
/// </summary>
/// <param name="key">The key.</param>
/// <param name="loadIfNotLoaded">if set to <c>true</c> [load if not loaded].</param>
/// <returns></returns>
std::shared_ptr<Atlas::RegisteredFile> Atlas::FileSystemRegistry::getFile(std::string key, bool loadIfNotLoaded) {
	uint16_t index = this->mLookupTable[key];
	std::shared_ptr<RegisteredFile> file = this->mRegisteredFiles[index];

	if (loadIfNotLoaded && !file->mIsLoaded) this->loadFile(file);

	return file;
}
