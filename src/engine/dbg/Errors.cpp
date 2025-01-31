#include "Errors.h"

Atlas::InvalidArgument::InvalidArgument(std::string const& message, std::source_location location) : InvalidValue(message, location) {
}

Atlas::InvalidValue::InvalidValue(std::string const& message, std::source_location location) : IException(message, location) {
}

Atlas::IException::IException(std::string const& message, std::source_location location) : mMessage(message) {
	//	Log(mMessage, ELogLevel::ERROR, location);
}

Atlas::IException::~IException() noexcept {
}

Atlas::IError::IError(std::string const& message, std::source_location location) : IException(message, location) {
}

Atlas::IError::~IError() noexcept {
}

Atlas::FileIOFailure::FileIOFailure(std::string const& message, std::source_location location) : IError(message, location) {
}

const char* Atlas::FileIOFailure::what() const noexcept {
	return mMessage.c_str(); 
}
