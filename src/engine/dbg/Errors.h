
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
	public:
		IException(std::string const& message, std::source_location location = std::source_location::current()) : mMessage(message) {
			//	Log(mMessage, ELogLevel::ERROR, location);
		}

		virtual ~IException() noexcept {}

		virtual const char* what() const noexcept = 0;

	protected:
		std::string mMessage;
	};

	class InvalidValue : public IException {
	public:
		ATLAS_DEFAULT_EXCEPTION_BODY(InvalidValue);
	};

	class InvalidArgument : public InvalidValue {
	public:
		ATLAS_CHILD_EXCEPTION_CONSTRUCTOR(InvalidArgument, InvalidValue);
	};
	
	/// <summary>
	/// Represents a base abstract class for errors.
	/// \warning Unlike an exception, an error is not recoverable!
	/// </summary>
	/// <seealso cref="IException" />
	class IError abstract : public IException {
	public:
		IError(std::string const& message, std::source_location location = std::source_location::current()) : IException(message, location) {}

		virtual ~IError() noexcept {}

		virtual const char* what() const noexcept = 0;
	};

	
	class FileIOFailure : public IError {
	public:
		FileIOFailure(std::string const& message, std::source_location location = std::source_location::current()) : IError(message, location) {}

		const char* what() const noexcept override { return mMessage.c_str(); }
	};


}