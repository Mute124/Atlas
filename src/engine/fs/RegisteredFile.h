/**
* @file RegisteredFile.h
* @todo Separate RegisteredFile into 2 classes: RegisteredFile and LoadedFile.
* @todo Make more thorough documentation of @ref Atlas::RegisteredFile.
*/
#pragma once
#include <any>
#include <memory>
#include <mutex>

#include "FileMeta.h"

namespace Atlas {
	
	

	/// <summary>
	/// Represents a file that has been registered with the FileSystemRegistry
	/// </summary>
	/// <seealso cref="Atlas::FileMeta" />
	/// <seealso cref="Atlas::FileSystemRegistry" />
	class RegisteredFile {
	private:
				
		/// <summary>
		/// A flag indicating if the file has been loaded
		/// </summary>
		/// <remarks>
		///    <c>true</c> if this instance is loaded; otherwise, <c>false</c>.
		/// </remarks>
		bool mIsLoaded = false;
		
		/// <summary>
		/// The file data as a std::any
		/// </summary>
		/// <remarks>
		/// This is a std::any that holds the loaded file data. However, if the file is not loaded, this will be a nullptr (this is how you can check if the file is loaded or not).
		/// </remarks>
		std::any mFileData = nullptr;
		
		/// <summary>
		/// The m file meta
		/// </summary>
		std::shared_ptr<FileMeta> mFileMeta = nullptr;
		
		/// <summary>
		/// The m index
		/// </summary>
		uint16_t mIndex = 0;
		
		/// <summary>
		/// The m load mutex
		/// </summary>
		std::mutex mLoadMutex = std::mutex();

	protected:		
		/// <summary>
		/// Gets the load mutex.
		/// </summary>
		/// <returns></returns>
		std::mutex& getLoadMutex() { return mLoadMutex; }

		friend class FileSystemRegistry;
	public:

		
		/// <summary>
		/// Default constructor
		/// </summary>
		RegisteredFile();
		
		/// <summary>
		/// Constructs this instance using file meta <seealso cref="FileMeta"/>.
		/// \note This is an explicit constructor
		/// </summary>
		/// <param name="meta">a const reference to the file meta that will be used to construct.</param>
		explicit RegisteredFile(std::shared_ptr<FileMeta> const& meta);

		/// <summary>
		/// Gets the underlining data of this file as the actual type it is stored as.
		/// </summary>
		/// <typeparam name="T">The type that the data is stored as.</typeparam>
		/// <returns>The casted file data</returns>
		template<typename T>
		T get() { return std::any_cast<T>(mFileData); }

		/// <summary>
		/// Gets the underlining data of this file as a std::any
		/// </summary>
		/// <returns>The std::any that holds the data</returns>
		std::any getData() { return mFileData; }

		/// <summary>
		/// Returns if this file is still just a registered file or actually loaded and usable by the code.
		///	</summary>
		/// <returns>Returns if this file is still just a registered file or actually loaded and usable by the code.</returns>
		bool isLoaded() { return mIsLoaded; }

		/// <summary>
		/// Get this file's mFileMeta
		///	</summary>
		/// <returns>The meta that is used by this file.</returns>
		std::shared_ptr<FileMeta> getFileMeta() { return mFileMeta; }

		/// <summary>
		/// Returns a uint16_t index that this is attached to. 
		///	</summary>
		/// <returns>The current index value that this is attached to.</returns>
		uint16_t getIndex() { return mIndex; }
	};
}