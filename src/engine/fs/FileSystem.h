#pragma once
#include <string>
#include <vector>
#include <memory>
#include <any>
#include <functional>

#include "FileMeta.h"
#include "RegisteredFile.h"
#include "FileSystemRegistry.h"
#include "../EReturnCode.h"

namespace Atlas {


	using DirectoryFiles = std::vector<std::string>;

	/// <summary>
	/// Checks if the file system is properly formatted.
	/// </summary>
	/// <param name="rootPath">The root path.</param>
	/// <param name="directoryMapPath">The directory map path.</param>
	/// <returns>A boolean value that denotes whether or not the file system is properly formatted.</returns>
	bool FileSystemIsProperlyFormatted(const char* rootPath, const char* directoryMapPath);

	/// <summary>
	/// Gets the file system registry.
	/// </summary>
	/// <returns>A reference to the current file system registry singleton</returns>
	FileSystemRegistry& GetFileSystemRegistry();

	/// <summary>
	/// Initializes the file registry.
	/// </summary>
	/// <param name="rootPath">The root path.</param>
	void InitializeFileRegistry(const char* rootPath);

	/// <summary>
	/// Adds an extension load function to the file registry
	/// </summary>
	/// <param name="extension">The extension.</param>
	/// <param name="loadFunc">The load function.</param>
	void AddFileRegistryLoadFunction(std::string extension, FileLoadFuncType loadFunc);

	/// <summary>
	/// Flags an extension as one that should be loaded once it is registered. This only matters and has effects before the file registry is initialized. 
	/// </summary>
	/// <param name="extension">The extension.</param>
	void AddFileRegistryLoadOnRegisterExtension(std::string const& extension);

	/// <summary>
	/// Gets the file meta.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <returns>A shared pointer to the found file meta </returns>
	std::shared_ptr<FileMeta> GetFileMeta(std::string key);

	/// <summary>
	/// Gets the file.
	/// </summary>
	/// <param name="key">The key.</param>
	/// <param name="loadIfNotLoaded">if set to <c>true</c> [load if not loaded].</param>
	/// <returns></returns>
	std::shared_ptr<RegisteredFile> GetFile(std::string key, bool loadIfNotLoaded = true);

	std::string GetFilePath(std::string key);
}
