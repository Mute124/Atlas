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

