// File system components
#include "FileMeta.h"
#include "FileSystemRegistry.h"
#include "RegisteredFile.h"

// Debug stuff
#include "../dbg/ELogLevel.h"
#include "../dbg/Errors.h"
#include "../dbg/Logging.h"

// Raylib is being included here because of the LoadDirectoryFilesEx function 
// and it has multiple useful functions
#include <raylib.h>

// Other includes
#include <any>
#include <cstdint>
#include <format>
#include <functional>
#include <memory>
#include <source_location>
#include <string>

#ifndef ATLAS_NO_PERFORMACE_LOGGING
	
#endif

uint16_t Atlas::FileSystemRegistry::GetNextFileIndex() { 
	return sFileIndex++; 
}

bool Atlas::FileSystemRegistry::isReady() const { 
	return sIsReady; 
}

uint16_t Atlas::FileSystemRegistry::getFileIndex(std::string const& key) const
{

/*	if(!this->mLookupTable.contains(key)) {
		return -1;
	} */

	return this->mLookupTable.at(key);
}

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

	std::string output = std::format("File with index[{}], with the meta of: {} was registered. ", index, meta.format());
	Log(output, ELogLevel::TRACE);
}

std::shared_ptr<Atlas::FileMeta> Atlas::FileSystemRegistry::GetFileMeta(std::string const& key)
{
	if (!sIsReady) {
		std::string registryNotReadyMsg = std::format("Registry is not ready; cannot get file meta: {}!", key);
		Log(registryNotReadyMsg, ELogLevel::WARNING);

		throw ImpatientUsageException(registryNotReadyMsg);

		return nullptr;
	}


/*	if(!this->mLookupTable.contains(key)) {
		std::string keyNotFoundMsg = std::format("File with key: {} was not found in the lookup table.", key);

		Log(keyNotFoundMsg, ELogLevel::WARNING);

		throw ValueNotFoundException(keyNotFoundMsg);

		return nullptr;
	}*/

	uint16_t index = getFileIndex(key);

	return this->mFileMetaRegistry[index];
}

void Atlas::FileSystemRegistry::loadFile(std::shared_ptr<RegisteredFile> file) {
	if (!sIsReady) {
		std::string registryNotReadyMsg = std::format("Registry is not ready; cannot load file: {}!", file->getFileMeta()->format());

		Log(registryNotReadyMsg, ELogLevel::WARNING);

		throw ImpatientUsageException(registryNotReadyMsg, std::source_location::current());
		return;
	}

	std::scoped_lock lock(file->getLoadMutex());
	std::shared_ptr<FileMeta> fileMeta = file->getFileMeta();

	if (fileMeta == nullptr) {
		Log("FileMeta is null!");
		return;
	}

	if (!mLoadingFunctions.contains(fileMeta->extension)) {
		Log("No load function found for extension: " + fileMeta->extension);
		
		throw MissingFileLoadFunctionException(fileMeta->extension, std::source_location::current());
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

		if (!file->mFileData.has_value()) {
			std::string msg = std::format("Failed to load file: {} because the load function returned null or empty data.", fileMeta->format());
			throw AccessViolationException(msg, std::source_location::current());
		}
	} catch(std::bad_any_cast& e) {
		
		std::string msg = std::format(
			"Failed to load file: {} because it was not of the expected type. Exception: {}",
			fileMeta->format(),
			e.what()
		);

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

bool Atlas::FileSystemRegistry::isFileRegistered(std::string const& key)
{
	return false;
}

void Atlas::FileSystemRegistry::addLoadFunction(std::string const& extension, FileLoadFuncType loadFunc) { 
	this->mLoadingFunctions[extension] = loadFunc; 
}

void Atlas::FileSystemRegistry::addLoadOnRegisterExtension(std::string const& extension) { 
	this->mLoadOnRegisterExtensionsSet.insert(extension); 
}

std::string Atlas::FileSystemRegistry::getFilePath(std::string const& key) {
	return this->mFileMetaRegistry[this->mLookupTable[key]]->path; 
}

Atlas::FileSystemRegistry::MissingFileException::MissingFileException(std::string const& message, std::source_location location)
	: InvalidValue(message, location) {}

Atlas::FileSystemRegistry::AccessViolationException::AccessViolationException(std::string const& message, std::source_location location)
	: IException(message, location)
{
}

const char* Atlas::FileSystemRegistry::AccessViolationException::what() const noexcept {
	return this->mMessage.c_str();
}
