#include "FileSystem.h"
#include <raylib.h>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include "../dbg/ELogLevel.h"
#include "../dbg/Logging.h"
#include <any>
#include <format>
#include <functional>



bool Atlas::FileSystemIsProperlyFormatted(const char* rootPath, const char* directoryMapPath)
{
	return false;
}

Atlas::FileSystemRegistry& Atlas::GetFileSystemRegistry()
{
	return FileSystemRegistry::Instance();
}

void Atlas::InitializeFileRegistry(const char* rootPath)
{
	GetFileSystemRegistry().init(rootPath);
}

void Atlas::AddFileRegistryLoadFunction(std::string  extension, std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc)
{
	GetFileSystemRegistry().addLoadFunction(extension, loadFunc);
}

void Atlas::AddFileRegistryLoadOnRegisterExtension(std::string const& extension)
{
	GetFileSystemRegistry().addLoadOnRegisterExtension(extension);
}

std::shared_ptr<Atlas::FileMeta> Atlas::GetFileMeta(std::string  key)
{
	return GetFileSystemRegistry().GetFileMeta(key);
}

std::shared_ptr<Atlas::RegisteredFile> Atlas::GetFile(std::string key, bool loadIfNotLoaded)
{
	return GetFileSystemRegistry().getFile(key, loadIfNotLoaded);
}

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

		// add to lookup table
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
