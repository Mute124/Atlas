/**************************************************************************************************
 * @file AException.h
 * 
 * @brief Provides the AException class.
 * 
 * @date February 2026
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <exception>
#include <stdexcept>
#include <string>
#include <source_location>
#include <type_traits>

#include "../core/Core.h"
#include "Logging.h"

namespace Atlas {

	template<class T>
	concept CustomException = std::is_base_of_v<std::exception, T>;

	/**
	 * @brief Provides information about an exception.
	 * 
	 * @note This class is not intended to be used outside of the AException class and it's subclasses, hence it cannot be
	 * inherited.
	 * 
	 * @since v0.0.1
	 */
	struct ExceptionInfo final {
		/**
		 * @brief The message of the exception.
		 * 
		 * @since v0.0.1
		 */
		const std::string message;

		/**
		 * @brief The location of @b where the exception was @b thrown.
		 * 
		 * @since v0.0.1
		 */
		const std::source_location throwLocation;
		
		/**
		 * @brief Constructs a new ExceptionInfo object from a C-string instead of @c std::string
		 * 
		 * @param message A C-string representing the message of the exception
		 * @param throwLocation A @c std::source_location object representing the location where the exception was thrown.
		 * If it is not provided, the default value is the current source location.
		 * 
		 * @since v0.0.1
		 */
		ATLAS_EXPLICIT ExceptionInfo(const char* message, const std::source_location throwLocation = std::source_location::current())
			: ExceptionInfo(std::string(message), throwLocation) {}

		/**
		 * @brief Constructs a new ExceptionInfo object from a @c std::string instead of a C-string.
		 * 
		 * @param message A @c std::string representing the message of the exception
		 * @param throwLocation A @c std::source_location object representing the location where the exception was thrown.
		 * If it is not provided, the default value is the current source location.
		 * 
		 * @since v0.0.1
		 */
		ATLAS_EXPLICIT ExceptionInfo(std::string const& message, const std::source_location throwLocation = std::source_location::current())
			: message(message), throwLocation(throwLocation) {}
		
		/**
		 * @brief Destroys the ExceptionInfo object using the default destructor.
		 * 
		 * @since v0.0.1
		 */
		~ExceptionInfo() = default;
	};

	template<CustomException T_CUSTOM_EXCEPTION>
	class ACustomException : public T_CUSTOM_EXCEPTION {
	private:

		const ExceptionInfo mExceptionInfo;

	public:

		ATLAS_EXPLICIT ACustomException(ExceptionInfo const& exceptionInfo)
			: T_CUSTOM_EXCEPTION(exceptionInfo.message.c_str()), mExceptionInfo(exceptionInfo) {}

		ATLAS_EXPLICIT ACustomException(std::string const& message, std::source_location throwLocation = std::source_location::current())
			: ACustomException(ExceptionInfo(message, throwLocation)) {}

		ATLAS_NODISCARD	std::string const& getMessage() const {
			return mExceptionInfo.message;
		}
	};

	class AException : public ACustomException<std::exception> {
	public:
		using ACustomException<std::exception>::ACustomException;

		void logException(ALogger* const logger) const {
			if (logger == nullptr) {
				return;
			}

			logger->log(getMessage(), ELogLevel::error);
		}
	};
}