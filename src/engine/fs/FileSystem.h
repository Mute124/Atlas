#pragma once
#include <string>
#include <vector>
#include <memory>
#include <any>
#include <functional>

#include "FileMeta.h"
#include "RegisteredFile.h"
#include "FileSystemRegistry.h"

namespace Atlas {
	using DirectoryFiles = std::vector<std::string>;
	bool FileSystemIsProperlyFormatted(const char* rootPath, const char* directoryMapPath);

	FileSystemRegistry& GetFileSystemRegistry();
	void InitializeFileRegistry(const char* rootPath);
	void AddFileRegistryLoadFunction(std::string extension, std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc);
	void AddFileRegistryLoadOnRegisterExtension(std::string const& extension);

	std::shared_ptr<FileMeta> GetFileMeta(std::string key);
	std::shared_ptr<RegisteredFile> GetFile(std::string key, bool loadIfNotLoaded = true);
}
