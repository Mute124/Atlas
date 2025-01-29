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
#include "../Project.h"


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
	GetAtlasEngine()->getFileSystemRegistry()->init(rootPath);
}


void Atlas::AddFileRegistryLoadFunction(std::string  extension, std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc)
{
	GetAtlasEngine()->getFileSystemRegistry()->addLoadFunction(extension, loadFunc);
}


void Atlas::AddFileRegistryLoadOnRegisterExtension(std::string const& extension)
{
	GetAtlasEngine()->getFileSystemRegistry()->addLoadOnRegisterExtension(extension);
}


std::shared_ptr<Atlas::FileMeta> Atlas::GetFileMeta(std::string  key)
{
	return GetAtlasEngine()->getFileSystemRegistry()->GetFileMeta(key);
}


std::shared_ptr<Atlas::RegisteredFile> Atlas::GetFile(std::string key, bool loadIfNotLoaded)
{
	return GetAtlasEngine()->getFileSystemRegistry()->getFile(key, loadIfNotLoaded);
}

