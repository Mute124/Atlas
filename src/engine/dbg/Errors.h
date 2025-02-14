#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <source_location>
#include "Logging.h"

#define ATLAS_DEFAULT_EXCEPTION_CONSTRUCTOR(exceptionType) \
	exceptionType(std::string const& message, std::source_location location = std::source_location::current()) : IException(message, location) {}

#define ATLAS_CHILD_EXCEPTION_CONSTRUCTOR(exceptionType, baseType) \
	exceptionType(std::string const& message, std::source_location location = std::source_location::current()) : baseType(message, location) {}

#define ATLAS_DEFAULT_EXCEPTION_BODY(exceptionType) \
	ATLAS_DEFAULT_EXCEPTION_CONSTRUCTOR(exceptionType) \
	const char* what() const noexcept override { return this->mMessage.c_str(); }

namespace Atlas {
	class IException : public std::exception {
	protected:
		std::string mMessage;
	public:				
		/// <summary>
		/// Initializes a new instance of the <see cref="IException"/> class.
		/// </summary>
		/// <param name="message">The message.</param>
		/// <param name="location">The location.</param>
		IException(std::string const& message, std::source_location location = std::source_location::current());
				
		/// <summary>
		/// Finalizes an instance of the <see cref="IException"/> class.
		/// </summary>
		/// <returns></returns>
		virtual ~IException() noexcept;
				
		/// <summary>
		/// Whats this instance.
		/// </summary>
		/// <returns></returns>
		virtual const char* what() const noexcept = 0;


	};
	
	/// <summary>
	/// 
	/// </summary>
	/// <seealso cref="IException" />
	class InvalidValue : public IException {
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="InvalidValue"/> class.
		/// </summary>
		/// <param name="message">The message.</param>
		/// <param name="location">The location.</param>
		InvalidValue(std::string const& message, std::source_location location = std::source_location::current());

		virtual const char* what() const noexcept override { return this->mMessage.c_str(); }
	};

	class InvalidArgument : public InvalidValue {
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="InvalidArgument"/> class.
		/// </summary>
		/// <param name="message">The message.</param>
		/// <param name="location">The location.</param>
		InvalidArgument(std::string const& message, std::source_location location = std::source_location::current());
	};

	/**
	 * @brief This exception is thrown when an ID is not found.
	 *
	 * @since v0.0.9
	 */
	class ValueNotFoundException : public InvalidArgument {
	public:
		using InvalidArgument::InvalidArgument;
	};

	class IError abstract : public IException {
	public:
		IError(std::string const& message, std::source_location location = std::source_location::current());

		virtual ~IError() noexcept;

		virtual const char* what() const noexcept = 0;
	};

	class FileIOFailure : public IError {
	public:
		FileIOFailure(std::string const& message, std::source_location location = std::source_location::current());

		const char* what() const noexcept override;
	};


}