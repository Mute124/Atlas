#include "../dbg/ELogLevel.h"
#include "../dbg/Logging.h"
#include "FileMeta.h"
#include "FileSystemRegistry.h"
#include "RegisteredFile.h"
#include "RegisteredFile.h"
#include <raylib.h>

#include <iostream>
#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <any>
#include <functional>

void Atlas::FileSystemRegistry::init(const char* rootPath)
{

	Log("Scanning " + std::string(rootPath) + " for files...");

	FilePathList files = LoadDirectoryFilesEx(rootPath, nullptr, true);

	Log("Found " + std::to_string(files.count) + " files.");

	for (int i = 0; i < files.count; i++) {
		std::string path = files.paths[i];
		if(FileExists(path.c_str())) {
			registerFile(path);
		} else {
			Log("File does not exist: " + path, ELogLevel::WARNING);
		}
		
	}
	sIsReady = true;
}

void Atlas::FileSystemRegistry::registerFile(std::string const& path)
{
	std::string filename = path.substr(path.find_last_of("\\") + 1);
	std::string extension = filename.substr(filename.find_last_of(".") + 1);

	// create file meta
	FileMeta meta(path);
/*	meta.path = path;
	meta.filename = filename;
	meta.extension = extension;
	meta.sandboxPath = meta.resolveSandboxPath(path);
	*/
	

	// Get file index
	uint16_t index = GetNextFileIndex();

	// add to configLookup table
	this->mLookupTable[meta.path] = index;
	this->mLookupTable[meta.filename] = index;
	this->mLookupTable[meta.extension] = index;
	this->mLookupTable[meta.sandboxPath] = index;

	// Register file
	this->mFileMetaRegistry[index] = std::make_shared<FileMeta>(meta);

	this->mRegisteredFiles[index] = std::make_shared<RegisteredFile>(std::make_shared<FileMeta>(meta));

	if (this->mLoadOnRegisterExtensionsSet.contains(extension)) {
		loadFile(this->mRegisteredFiles[index]);
	}

	std::string output = "";

	output = std::format("File with index[{}], with the meta of: {} was registered. ", index, meta.format());
	Log(output, ELogLevel::TRACE);
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
	if (!sIsReady) {
		std::string registryNotReadyMsg = std::format("Registry is not ready; cannot load file: {}!", file->getFileMeta()->format());

		Log(registryNotReadyMsg, ELogLevel::WARNING);
		return;
	}

	std::scoped_lock lock(file->getLoadMutex());
	std::shared_ptr<FileMeta> fileMeta = file->getFileMeta();

	if (fileMeta == nullptr) {
		Log("FileMeta is null!");
		return;
	}


	Log("Loading file: " + fileMeta->filename + " (" + fileMeta->extension + ")", ELogLevel::TRACE);
	FileLoadFuncType loadFunc = mLoadingFunctions.at(fileMeta->extension);

	if (loadFunc == nullptr) {
		Log("No load function found for extension: " + fileMeta->extension);
		return;
	}

	try {
		file->mFileData = loadFunc(fileMeta);
	} catch(std::bad_any_cast& e) {
		
		std::string msg = std::format("Failed to load file: {} because it was not of the expected type.", fileMeta->format());
		Log(msg, ELogLevel::ERROR);
		file->mFileData = nullptr;

		return;
	}

	file->mIsLoaded = true;
}

std::shared_ptr<Atlas::RegisteredFile> Atlas::FileSystemRegistry::getFile(std::string const& key, bool loadIfNotLoaded) {
	
	if (!sIsReady) {
		std::string registryNotReadyMsg = std::format("Registry is not ready; cannot get file: {}!", key);
		Log(registryNotReadyMsg, ELogLevel::WARNING);
		return nullptr;
	}

	uint16_t index = this->mLookupTable[key];
	std::shared_ptr<RegisteredFile> file = this->mRegisteredFiles[index];

	if (file == nullptr) {
		std::string keyNotFoundMsg = std::format("File with key: {} was not found.", key);
		Log(keyNotFoundMsg, ELogLevel::WARNING);
		return nullptr;
	}

	if (loadIfNotLoaded && !file->mIsLoaded) {
		this->loadFile(file);
	}
	return file;
}

std::string Atlas::FileSystemRegistry::getFilePath(std::string const& key) { 
	return this->mFileMetaRegistry[this->mLookupTable[key]]->path; 
}