/**
* @file FileSystemRegistry.h
* @todo Write documentation
* @todo Write tests
* @todo Clean up the code to reduce complexity. Perhaps break into smaller classes.
*/
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
	
	/// <summary>
	/// The file system registry. 
	/// </summary>
	/// <remarks>
	/// The file system registry is a relatively complex class because it:
	/// <list type="bullet">
	///		<item>
	///			<description>Keeps track of files.</description>
	///		</item>
	///		<item>
	///			<description>Handles loading of files</description>
	///		</item>
	///		<item>
	///			<description>Handles unloading of files</description>
	///		</item>
	///		<item>
	///			<description>Handles registration of files</description>
	///		</item>
	///		<item>
	///			<description>And just about everything else that a registry does</description>
	///		</item>
	/// </list>
	/// <par/>
	/// As an example of how to add a new file extension, see the \ref file-loading document.
	/// </remarks>
	/// <seealso cref="Singleton&lt;FileSystemRegistry&gt;" />
	class FileSystemRegistry : public Singleton<FileSystemRegistry> {
	private:		
		/// <summary>
		/// This static uint16_t variable represents the next available file index.
		/// </summary>
		/// <remarks>
		/// This is typically only used during the discovery and registration of files. It is used to assign a unique index to each file meta.
		/// </remarks>
		static inline uint16_t sFileIndex = 0;
		
		/// <summary>
		/// Gets the next file index
		/// </summary>
		/// <code>
		/// int a = 0;
		/// </code>
		/// <returns>The next file index</returns>
		static inline uint16_t GetNextFileIndex() { return sFileIndex++; }

		/// <summary>
		/// Holds the functions that will be called when a file is loaded.
		/// </summary>
		/// <remarks>
		/// When a file is loaded, the respective load function will be called. The string key is the file extension, and the function is the load function. As is documented in the
		/// <see cref="file-loading" />, each extension has its own load function or else Atlas does not know how to load the file. 
		/// </remarks>
		std::unordered_map<std::string, std::function<std::any(std::shared_ptr<FileMeta>)>> mLoadingFunctions;
		
		/// <summary>
		/// Tells the registry which extensions should be loaded on register. If it exists in the set, it will be loaded
		/// </summary>
		/// <remarks>
		/// This is set PRIOR to the discovery of files because this defines what extensions should be immediately loaded on registration. This is a set because this is only used
		/// to determine if a file should be loaded on register. The code knows if the file should be loaded on register by checking if the extension is contained in the set.
		/// </remarks>
		std::unordered_set<std::string> mLoadOnRegisterExtensionsSet;
		
		/// <summary>
		/// An unordered map that maps a file index to a registered file (stored as a shared_ptr).
		/// </summary>
		std::unordered_map<uint16_t, std::shared_ptr<RegisteredFile>> mRegisteredFiles;
		
		/// <summary>
		/// An unordered map that maps a file index to a file meta (stored as a shared_ptr).
		/// </summary>
		/// <remarks>
		/// This is the lookup table for the file meta. The uint16_t index is the output from the search in mLookupTable.
		/// </remarks>
		std::unordered_map<uint16_t, std::shared_ptr<FileMeta>> mFileMetaRegistry;
		
		/// <summary>
		/// An unordered map that maps a file path to a file index. This essentially serves as a reverse lookup table that maps a key to an index. 
		/// </summary>
		/// <remarks>
		/// The key can be either the file path, the file name, the file extension, or really anything that can be used to identify a file. However, Atlas only uses file path, 
		/// the file name, and the file extension to identify a file. This is the first step in the process of looking up a file. The uint16_t index corresponds to the index in 
		/// the mFileMetaRegistry unordered map. 
		/// </remarks>
		std::unordered_map<std::string, uint16_t> mLookupTable;

		
		friend class IProject;
	public:
		
		/// <summary>
		/// Initializes a new instance of the <see cref="FileSystemRegistry"/> class.
		/// </summary>
		FileSystemRegistry() {}

		/// <summary>
		/// Initializes the specified root path.
		/// </summary>
		/// <remarks>
		/// This function is used to initialize the file system registry. It will scan the provided root path for files and add them to the registry. The recursive searching method used is LoadDirectoryFilesEx from
		/// Raylib.
		/// </remarks>
		/// <param name="rootPath">The root path.</param>
		void init(const char* rootPath);
		
		/// <summary>
		/// Registers a file into the file registry.
		/// </summary>
		/// <remarks>
		/// This function creates a new FileMeta object from the provided path, 
		/// generates a unique index for the file, and adds it to the registry's lookup table. If the file's extension is in the load on register extensions set, it will be loaded immediately. This function is mostly used
		/// internally within the <see cref="FileSystemRegistry::init" /> function during the file registration process.
		/// </remarks>
		/// <param name="path">The path to the file that should be registered. This is the exact path to the file, not the file name.</param>
		void registerFile(std::string const& path);

		/// <summary>
		/// Gets the file meta stored in the file registry under the specified key.
		/// </summary>
		/// <param name="key">The filename, file path, file extension, or anything else that was used to identify a file upon registration.</param>
		/// <returns>A shared_ptr to the file meta stored.</returns>
		std::shared_ptr<FileMeta> GetFileMeta(std::string const& key);

		/// <summary>
		/// Loads a file.
		/// </summary>
		/// <remarks>
		/// Turns a registered file into a loaded file. Typically, this is called internally within the <see cref="FileSystemRegistry::registerFile" /> function, but is also called when a file is used for the first time.
		/// </remarks>
		/// <param name="file">The file shared_ptr to load.</param>
		void loadFile(std::shared_ptr<RegisteredFile> file);

		/// <summary>
		/// Gets the file.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="loadIfNotLoaded">if set to <c>true</c> [load if not loaded].</param>
		/// <returns></returns>
		std::shared_ptr<RegisteredFile> getFile(std::string const& key, bool loadIfNotLoaded = true);

		/// <summary>
		/// Adds the load function.
		/// </summary>
		/// <param name="extension">The extension.</param>
		/// <param name="loadFunc">The load function.</param>
		void addLoadFunction(std::string const& extension, std::function<std::any(std::shared_ptr<FileMeta>)> loadFunc) { this->mLoadingFunctions[extension] = loadFunc; }

		/// <summary>
		/// Adds the load on register extension.
		/// </summary>
		/// <param name="extension">The extension.</param>
		void addLoadOnRegisterExtension(std::string const& extension) { this->mLoadOnRegisterExtensionsSet.insert(extension); }


	};
}