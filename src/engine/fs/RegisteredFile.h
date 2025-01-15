#pragma once
#include <any>
#include <memory>
#include <mutex>

#include "FileMeta.h"

namespace Atlas {
	
	class RegisteredFile {
	private:
		bool mIsLoaded = false;
		std::any mFileData = nullptr;
		std::shared_ptr<FileMeta> mFileMeta = nullptr;
		uint16_t mIndex = 0;
		std::mutex mLoadMutex = std::mutex();

	protected:
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