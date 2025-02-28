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

#include "../dbg/Errors.h"

#include "../utils/Singleton.h"
#include <source_location>

namespace Atlas {
	using FileLoadFuncType = std::function<std::any(std::shared_ptr<FileMeta>)>;
	
	/**
	 * @brief The file system registry. 
	 * 
	 * @remarks The file system registry is a relatively complex class because it:
	 * <list type="bullet">
	 * 		<item>
	 * 			<description>Keeps track of files.</description>
	 * 		</item>
	 * 		<item>
	 * 			<description>Handles loading of files</description>
	 * 		</item>
	 * 		<item>
	 * 			<description>Handles unloading of files</description>
	 * 		</item>
	 * 		<item>
	 * 			<description>Handles registration of files</description>
	 * 		</item>
	 * 		<item>
	 * 			<description>And just about everything else that a registry does</description>
	 * 		</item>
	 *  </list>
	 * As an example of how to add a new file extension, see the \ref file-loading document.
	 * 
	 * @since v0.0.4
	 * 
	 * @todo Clean this up, it is a complete mess. 
	 * @todo Remove the singleton
	 * @todo Add unit tests
	 * 
	 * @sa @ref Singleton
	 */
	class FileSystemRegistry : public Singleton<FileSystemRegistry> {
	private:		
		friend class BProject;

		static inline volatile bool sIsReady = false;

		/**
		 * @brief This static uint16_t variable represents the next available file index.
		 * 
		 * @remarks This is typically only used during the discovery and registration of files. It is 
		 * also used to quickly assign a unique index to each file meta.
		 * 
		 * @since v0.0.4
		 */
		static inline uint16_t sFileIndex = 0;

		/**
		 * @brief Holds the functions that will be called when a file is loaded. When a file is loaded, 
		 * the respective extension's load function will be called. The string key is the file extension,
		 * and the function is the load function. As is documented in the @ref "file-loading", each extension
		 * has to have it's own load function or else Atlas does not know how to load that specific filetype.
		 * This is where the whole concept of extension-based loading it comes into play.
		 * 
		 * @since v0.0.4
		 * 
		 * @sa @ref file-loading
		 */
		std::unordered_map<std::string, FileLoadFuncType> mLoadingFunctions;

		/**
		 * @brief An unordered map that maps a file index to a registered file (stored as a shared_ptr).
		 * 
		 * @since v0.0.4
		 */
		std::unordered_map<uint16_t, std::shared_ptr<RegisteredFile>> mRegisteredFiles;

		/**
		 * @brief An unordered map that maps a file index to a file meta shared_ptr (see @ref FileMeta). This is the lookup
		 * table for the file meta. The uint16_t index is the output from the search in mLookupTable.
		 * 
		 * @since v0.0.4
		 * 
		 * @sa @ref mLookupTable for how the lookup table works.
		 * @sa @ref FileMeta for what a file meta is.
		 */
		std::unordered_map<uint16_t, std::shared_ptr<FileMeta>> mFileMetaRegistry;

		/**
		 * @brief An unordered map that maps a file path to a file index. This essentially serves as a reverse lookup table
		 * that maps a key to an index. The key can be either the file path, the file name, the file extension, or really
		 * anything that can be used to identify a file. However, Atlas only uses file path, the file name, and the file
		 * extension to identify a file. This is the first step in the process of looking up a file. The uint16_t index
		 * corresponds to the index in the @ref mFileMetaRegistry unordered map.
		 * 
		 * @since v0.0.4
		 */
		std::unordered_map<std::string, uint16_t> mLookupTable;

		/**
		 * @brief Tells the registry which extensions should be loaded on register. If it exists in the set,
		 * it will be loaded.
		 *
		 * @note This is set PRIOR to the discovery of files because this defines what extensions should be
		 * immediately loaded on registration. This is a set because this is only used to determine if a file
		 * should be loaded on register. The code knows if the file should be loaded on register by checking
		 * if the extension is contained in the set.
		 *
		 * @since v0.0.4
		 */
		std::unordered_set<std::string> mLoadOnRegisterExtensionsSet;

		/**
		 * @brief Gets the next file index
		 *
		 * @return The next file index
		 *
		 * @since v0.0.4
		 *
		 * @sa @ref sFileIndex
		 */
		static inline uint16_t GetNextFileIndex();

		/**
		 * @brief A getter function that tells you if the registry is ready or not; this
		 * just returns @ref sIsReady. 
		 * 
		 * @return The copied value of @ref sIsReady
		 * 
		 * @since v0.0.9
		 * 
		 * @sa @ref sIsReady for what sIsReady is
		 */
		bool isReady() const;

		/**
		 * @brief Searches the lookup table to find the index of a file.
		 * 
		 * @param key One of the keys that was used to register the file. 
		 * 
		 * @return A uint16_t that corresponds to the index of the file in the registry.
		 * 
		 * @since v0.0.9
		 */
		uint16_t getFileIndex(std::string const& key) const;
	public:




		/**
		 * @brief An exception that is thrown when a file is not found where it should be.
		 * This can mean that the key being used to search for a file is incorrect, or that
		 * the file is not registered. If this exception is thrown, it is recommended that
		 * you ensure that you have the correct key(s) being used to search for the file
		 * and that the file exists within the sandbox.
		 *
		 * @since v0.0.9
		 *
		 * @sa @ref FileSystemRegistry For where this exception is mainly thrown
		 */
		class MissingFileException : public InvalidValue {
		public:

			MissingFileException(std::string const& message, std::source_location location = std::source_location::current());
		};

		class AccessViolationException : public IException {
		public:
			AccessViolationException(std::string const& message, std::source_location location = std::source_location::current());

			const char* what() const noexcept override;
		};

		/**
		 * @brief An exception that is thrown when there is no registered load
		 * function for the file extension of the file that is being loaded.
		 *
		 * @since v0.0.9
		 */
		class MissingFileLoadFunctionException final : public AccessViolationException {
		public:
			using AccessViolationException::AccessViolationException;
		};

		/**
		 * @brief What is meant by "impatient" is that a piece of code has
		 * used this class when @c sIsReady is @c false. Essentially, this
		 * is thrown when some code is being impatient and not waiting for
		 * the registry to be ready. This class inherits from @ref AccessViolationException
		 * 
		 * @since v0.0.9
		 */
		class ImpatientUsageException : public AccessViolationException {
		public:
			using AccessViolationException::AccessViolationException;
		};

		/**
		 * @brief This exception is thrown when a piece of code tries to
		 * search for a file that is not allowed to be searched for. This
		 * class inherits from @ref AccessViolationException.
		 *
		 * @since v0.0.9
		 */
		class ForbiddenFileException : public AccessViolationException {
		public:
			using AccessViolationException::AccessViolationException;
		};

		/**
		 * @brief This is thrown when a piece of code tries to register, load,
		 * unload, or search for a file that is out of the sandbox. This class
		 * inherits from @ref ForbiddenFileException because this is a type of 
		 * forbidden access violation.
		 * 
		 * @since v0.0.9
		 */
		class FileOutOfSandboxException : public ForbiddenFileException {
		public:
			/**
			 * @copydoc ForbiddenFileException::ForbiddenFileException
			 * 
			 * @since v0.0.9
			 */
			using ForbiddenFileException::ForbiddenFileException;
		};

		/**
		 * @brief This is thrown when a piece of code tries to search for a file
		 * that does not exist, on a blacklist, or is otherwise forbidden.
		 * This class inherits from @ref ForbiddenFileException because this is a
		 * type of forbidden access violation.
		 * 
		 * @since v0.0.9
		 */
		class ForbiddenFileSearchException : public ForbiddenFileException {
		public:

			/**
			 * @copydoc ForbiddenFileException::ForbiddenFileException
			 * 
			 * @since v0.0.9
			 */
			using ForbiddenFileException::ForbiddenFileException;
		};

		/**
		 * @brief Initializes a new instance of the @ref FileSystemRegistry class.
		 * 
		 * @since v0.0.4
		 */
		FileSystemRegistry() {}

		/**
		 * @brief This function is used to initialize the file system registry. It will scan the
		 * provided root path for files and add them to the registry. The recursive searching method
		 * used is LoadDirectoryFilesEx from Raylib.
		 * 
		 * @param rootPath The path to the directory that should be scanned. For example, the "game"
		 * directory.
		 * 
		 * @since v0.0.4
		 * 
		 * @todo Encapsulate this function
		 */
		void init(const char* rootPath);

		/**
		 * @brief Registers a file into the file registry using the provided path by creating a new @ref FileMeta
		 * object, generating a unique index for the file, and adding it to the registry's lookup table. If the
		 * file's extension is in the load on register extensions set (@ref mLoadOnRegisterExtensionsSet), it will 
		 * be loaded immediately.
		 *
		 * @remarks This function is mostly used internally within the @ref init function during the
		 * file registration process, but has been made public because it can still be used to register
		 * files that were not registered during the initialization process.
		 * 
		 * @param path The path to the file that should be registered. This is the exact path to the file, 
		 * not the file name.
		 * 
		 * @since v0.0.4
		 */
		void registerFile(std::string const& path);
		
		/**
		 * @brief Gets the file meta stored in the file registry under the specified key.
		 * 
		 * @param key The filename, file path, file extension, or anything else that was used to identify a
		 * file upon registration.
		 * 
		 * @return A shared_ptr to the file meta stored within the file registry. 
		 * 
		 * @since v0.0.4
		 * 
		 * @sa @ref FileMeta for what file meta is
		 * @sa @ref mFileMetaRegistry for where the file meta is stored at.
		 */
		std::shared_ptr<FileMeta> GetFileMeta(std::string const& key);

		/**
		 * @brief Turns a registered file into a loaded file. Typically, this is
		 * called internally within the @ref registerFile function, but is also
		 * called when a file is used for the first time.
		 * 
		 * @param file A shared_ptr to the [registered] file that should be loaded.
		 * 
		 * @throws ImpatientUsageException If this is called before the registry is ready
		 * 
		 * @since v0.0.4
		 */
		void loadFile(std::shared_ptr<RegisteredFile> file);

		/**
		 * @brief Looks up a file in the file registry. Unless @ref loadIfNotLoaded is set
		 * to false, this function @b WILL load the file if it is not already loaded.
		 * 
		 * @param key The key that was used to register the file and is stored in the lookup
		 * table as.
		 * 
		 * @param loadIfNotLoaded If set to @c true the file will be loaded if it is not
		 * already loaded. Please note that this is set to @c true by default.
		 * 
		 * @return A shared_ptr to the file that was found in the file registry.
		 * 
		 * @since v0.0.4
		 */
		std::shared_ptr<RegisteredFile> getFile(std::string const& key, bool loadIfNotLoaded = true);

		/**
		 * @brief Checks within the registry if a file has been registered by looking up
		 * the key in the lookup table.
		 * 
		 * @param key The key that was used to register the file and is stored in the lookup
		 * table as. 
		 * 
		 * @return @c true if the file has been registered, @c false if not.
		 */
		bool isFileRegistered(std::string const& key);

		/**
		 * @brief Adds a load function for a specific extension. This is how you may inform Atlas 
		 * on how to load a specific file type. This function is also a "call-and-forget" function;
		 * Atlas handles the loading of the file for you and there is not much you need to do 
		 * afterwards.
		 * 
		 * @param extension This defines what file type the load function is for. Keep in mind that
		 * this is @b case-sensitive and is @b WITHOUT the leading "." in the extension.
		 * 
		 * @param loadFunc A lambda function that takes a shared_ptr to the file meta as a parameter
		 * and returns a shared_ptr to the file data.
		 * 
		 * @since v0.0.4
		 * 
		 * @sa @ref FileLoadFuncType
		 */
		void addLoadFunction(std::string const& extension, FileLoadFuncType loadFunc);

		/**
		 * @brief This marks a certain extension as one that should be loaded when a file with
		 * the specified extension is registered using the concept of immediate loading (refer
		 * to the main page for more information). This must be done before a file(s) get
		 * registered. The way this works is by adding the extension to the @ref mLoadOnRegisterExtensionsSet
		 * set.
		 * 
		 * @note It is recommended that you do this in your project's @c preInit function.
		 * 
		 * @param extension The extension that should be flagged for loading in the
		 * @ref mLoadOnRegisterExtensionsSet.
		 * 
		 * @since v0.0.4
		 */
		void addLoadOnRegisterExtension(std::string const& extension);

		std::string getFilePath(std::string const& key);
	};
}