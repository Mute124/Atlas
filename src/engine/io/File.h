#pragma once
#include <any>
#include <string>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iosfwd> // This is just to avoid a transitive include for std::fstream

#include "IOCommon.h"

namespace Atlas {
	using FileStream = std::fstream;

	/**
	 * @brief A struct that holds the result of a file cast.
	 * 
	 * @tparam T_DATA_STORAGE_TYPE The file's data storage type.
	 * 
	 * @since v0.0.1
	 */
	template<typename T_DATA_STORAGE_TYPE>
	struct FileCastAttempt {

		/**
		 * @brief The file's data storage type.
		 * 
		 * @since v0.0.1
		 */
		T_DATA_STORAGE_TYPE castResult;
		
		/**
		 * @brief Whether the cast was successful.
		 * 
		 * @since v0.0.1
		 */
		bool wasSuccessful = false;
	};


	/**
	 * @brief An abstract class that represents a file and its data.
	 * 
	 * @note This cannot be used directly, it is an abstract class.
	 * 
	 * @since v0.0.1
	 */
	class AFile {
	private:
		
		/**
		 * @brief The file's data represented as a std::any object.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref castAs() for casting the file's data into a more usable type.
		 */
		std::any mFileData;

		/**
		 * @brief The path location to the file on the user's disk.
		 * 
		 * @since v0.0.1
		 */
		PathLocation mFilePath;

		/**
		 * @brief How many bytes the file contains.
		 * 
		 * @since v0.0.1
		 */
		size_t mFileSize = 0;

		/**
		 * @brief A number that does not have a specific meaning other than to provide other information about the file.
		 * 
		 * @since v0.0.1
		 */
		uint16_t mFileType = -1;

		/**
		 * @brief Whether or not the file has been loaded.
		 * 
		 * @since v0.0.1
		 */
		bool mIsLoaded = false;

	protected:

		// The below functions are protected because they are meant to be used internally by derived classes only.

		/**
		 * @brief Sets the file's path.
		 * 
		 * @param cPath What to set the file's path to.
		 * 
		 * @note This will not check if the path is valid, and it should only really be used internally. This does not affect the file's data.
		 * 
		 * @since v0.0.1
		 */
		void setFilePath(PathLocation const& cPath);
		
		/**
		 * @brief Sets the file's size in bytes.
		 * 
		 * @param cSize How big the file is.
		 * 
		 * @since v0.0.1
		 */
		void setFileSize(size_t cSize);

		/**
		 * @brief Sets the file's type.
		 * 
		 * @param cType The file's type.
		 * 
		 * @note Refer to the note on @ref mFileType for why this exists.
		 * 
		 * @since v0.0.1
		 */
		void setFileType(uint16_t cType);

		/**
		 * @brief Sets whether or not the file has been loaded.
		 * 
		 * @param bIsLoaded Whether or not the file has been loaded.
		 * 
		 * @since v0.0.1
		 */
		void setIsLoaded(bool bIsLoaded);

		/**
		 * @brief Sets the file's data.
		 * 
		 * @param cData The file's @b loaded data.
		 * 
		 * @note This will not check if the data is valid (or if the data exists at all), so be careful.
		 * 
		 * @since v0.0.1
		 */
		void setFileData(std::any const& cData);

		/**
		 * @brief A protected constructor that is meant to be used by derived classes.
		 * 
		 * @param cPath What the file's path is.
		 * @param cSize How big the file is.
		 * @param cType The file's type.
		 * @param bIsLoaded Whether or not the file has been loaded.
		 * 
		 * @since v0.0.1
		 */
		AFile(PathLocation const& cPath, size_t cSize, uint16_t cType, bool bIsLoaded);

	public:
		
		/**
		 * @brief A default constructor.
		 * 
		 * @since v0.0.1
		 */
		AFile() = default;

		/**
		 * @brief A default destructor.
		 * 
		 * @since v0.0.1
		 */
		~AFile() = default;

		FileStream open(PathLocation const& cFilePath);

		/**
		 * @brief Loads the file's data.
		 * 
		 * @param cPath The path to the file on the user's disk.
		 * 
		 * @note This function is meant to be overridden by derived classes if they need to load the file's data in a different way.
		 * 
		 * @since v0.0.1
		 */
		virtual uint16_t load(PathLocation const& cPath);

		virtual uint16_t append(std::string const& cData);

		/**
		 * @brief A pure virtual function that parses the file's data.
		 * 
		 * @note This must be overridden by derived classes since different file types have different data structures, therefore different parsing methods.
		 * 
		 * @since v0.0.1
		 */
		virtual uint16_t parse() = 0;

		/**
		 * @brief A pure virtual function that checks if the file is valid.
		 * 
		 * @note As a form of guidance, this usually checks if the file exists on the user's disk. Furthermore, this function should also check if the file's data is valid or not.
		 * 
		 * @return @a bool @a true if the file is valid, @a false otherwise.
		 * 
		 * @since v0.0.1
		 */
		virtual bool isValid() const = 0;

		/**
		 * @brief Returns the value of @ref mIsLoaded.
		 * 
		 * @return @a bool The value of @ref mIsLoaded.
		 * 
		 * @since v
		 */
		bool isLoaded() const;

		/**
		 * @brief Returns the value of @ref mFilePath.
		 * 
		 * @return @a PathLocation The value of @ref mFilePath.
		 * 
		 * @since v0.0.1
		 */
		PathLocation const& getFilePath() const;

		/**
		 * @brief Returns the value of @ref mFileSize.
		 * 
		 * @return @a size_t The value of @ref mFileSize.
		 * 
		 * @since v0.0.1
		 */
		size_t getFileSize() const;

		/**
		 * @brief Returns the value of @ref mFileType.
		 * 
		 * @return @a uint16_t The value of @ref mFileType.
		 * 
		 * @since v0.0.1
		 */
		uint16_t getFileType() const;

		/**
		 * @brief Returns the value of @ref mFileData.
		 * 
		 * @return @a std::any The value of @ref mFileData.
		 * 
		 * @since v0.0.1
		 */
		std::any const& getFileData() const;

		/**
		 * @brief Casts the file's data to a different type.
		 * 
		 * @tparam T_CONVERT_TO What to cast the file's data to.
		 * 
		 * @important Please ensure that the file's data can be cast to @a T_CONVERT_TO and that there is a way to do so.
		 * 
		 * @return The casted data.
		 * 
		 * @since v0.0.1
		 */
		template<typename T_CONVERT_TO>
		T_CONVERT_TO castAs() const {
			return std::any_cast<T_CONVERT_TO>(mFileData);
		}

		// Operators

		/**
		 * @brief An @b explicit and @b const operator that returns the size of the file.
		 * 
		 * @return @a size_t The size of the file.
		 * 
		 * @since v0.0.1
		 */
		explicit operator size_t() const;

		/**
		 * @brief An @b implicit and @b const operator that returns @a true if the file is valid, @a false otherwise.
		 * 
		 * @return @a bool @a true if the file is valid, @a false otherwise.
		 * 
		 * @since v0.0.1
		 */
		explicit(false) operator bool() const;
	};

	bool PathExists(PathLocation const& cPath);
}
