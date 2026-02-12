/**************************************************************************************************
 * @file Logging.h
 * 
 * @brief This file provides a standard logging system that Atlas uses. This file also has some configurable
 * preprocessor defines that can customize the behavior of the logging system. Below is the list of these
 * preprocessor defines:
 * - @c ATLAS_NO_SPDLOG | If defined, Atlas will not use the spdlog logger (which is the default logger).
 * 
 * 
 * @note For most cases, you should use @ref Atlas::SpdlogLogger as your logger. 
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <string>
#include <memory>
#include <initializer_list>
#include <mutex>
#include <source_location>
#include <queue>
#include <any>

#ifndef ATLAS_NO_SPDLOG
	#include <spdlog/logger.h>
	#include <spdlog/sinks/stdout_color_sinks.h>
	#include <spdlog/sinks/basic_file_sink.h>
	#include <spdlog/common.h>


	// These defines abstract away the spdlog level enums so that the user is not forced
	// to use the spdlog level enums.
	#define ATLAS_LEVEL_TRACE SPDLOG_LEVEL_TRACE
	#define ATLAS_LEVEL_DEBUG SPDLOG_LEVEL_DEBUG
	#define ATLAS_LEVEL_INFO SPDLOG_LEVEL_INFO
	#define ATLAS_LEVEL_WARN SPDLOG_LEVEL_WARN
	#define ATLAS_LEVEL_ERROR SPDLOG_LEVEL_ERROR
	#define ATLAS_LEVEL_CRITICAL SPDLOG_LEVEL_CRITICAL
	#define ATLAS_LEVEL_OFF SPDLOG_LEVEL_OFF
		
	#define ATLAS_DEFAULT_SPDLOG_LOG_PATTERN "%c [%n][%t] [%^%l%$] %v"

	// Make sure that ATLAS_USE_SPDLOG is defined.
	#ifndef ATLAS_USE_SPDLOG
#define ATLAS_USE_SPDLOG
	#endif

#endif

#include "../core/Common.h"

namespace Atlas {

#ifdef ATLAS_USE_SPDLOG
	using SpdlogLogLevel = spdlog::level::level_enum;
#endif

	enum class ELogLevel : int {
		trace = ATLAS_LEVEL_TRACE,
		debug = ATLAS_LEVEL_DEBUG,
		info = ATLAS_LEVEL_INFO,
		warn = ATLAS_LEVEL_WARN,
		error = ATLAS_LEVEL_ERROR,
		critical = ATLAS_LEVEL_CRITICAL,
		off = ATLAS_LEVEL_OFF,
		nLevels
	};

#ifdef ATLAS_USE_SPDLOG
	enum class ESpdlogInitListIndex : int {
		ConsoleLogger = 0,
		FileLogger = 1,
		Logger = 2,
		Max = Logger
	};

	enum class ESpdlogSinkType : int {
		Console = 0,
		File = 1
	};

#endif

	//std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages,
	//	std::initializer_list<ELogLevel> sinksLogLevels
	class LogMessage {
	public:
		std::string message;
		ELogLevel logLevel;
		std::source_location eventLocation;
		std::optional<std::any> extraData;
	};

	/**
	 * @brief Interface for logging that is responsible for handling any logging from the engine, game, or mods.
	 *
	 * @since v0.0.1
	 */
	class ALogger : public Initializable {
	public:
		

		ALogger() = default;

		virtual ~ALogger() = default;

		static std::string GenerateLogFileName();

		static void Log(std::string const& message, ELogLevel logLevel);

		virtual void init() = 0;

		virtual void close() = 0;

		virtual void display(std::string const& message) {}

		virtual void log(std::string const& message, ELogLevel logLevel) {}
	
		virtual void setLevel(ELogLevel logLevel) {}
	};

	/**
	 * @brief A generic logger that uses the @b Spdlog library. According to the spdlog documentation, you may call the
	 * @a spdlog::log() function to log messages @b after the logger has been initialized because of how spdlog works.
	 * For more information, on how to use spdlog, please refer to their documentation. This class was designed to work
	 * in the same way as the multi-sink logger example provided in the spdlog documentation. Another important thing to
	 * note about this class is that, assuming you are not implementing a custom @c PFN_vkDebugUtilsMessengerCallbackEXT
	 * callback for Vulkan, this class should be initialized prior to the initialization and usage of Atlas' Vulkan API.
	 * Otherwise, you will get errors the second Vulkan starts sending messages to the callback.
	 * 
	 * @details To initialize this class in a case where you immediately have the initialization variables, consider the
	 * following code:
	 * 
	 * @code{.cpp}
	 * SpdlogLogger logger = SpdlogLogger("Atlas", "[Some Logger] [%^%l%$] %v", "log.txt");
	 * logger.init();
	 * @endcode
	 * 
	 * If you do not have the initialization variables but need to set them later, you may make @c logger a pointer to
	 * get around the lack of a default constructor. However, keep in mind that a default constructor may be implemented
	 * later on.
	 * 
	 * @note While most of the variables that are required for initialization within this class, there are a couple
	 * that must absolutely be set because there is no default value for them. More specifically, the values of the
	 * following variables that do not have a default value must be set PRIOR to calling @ref init():
	 * - @ref mMessageFormatPattern
	 * - @ref mLogFilePath
	 * Please refer to the documentation of those variables for more information about them.
	 *
	 * @since v0.0.1
	 * 
	 * @sa @ref ALogger The interface that this class publicly implements.
	 * 
	 * @sa https://github.com/gabime/spdlog?tab=readme-ov-file#logger-with-multi-sinks---each-with-a-different-format-and-log-level
	 * for what this class' main functionality is based on.
	 * 
	 * @todo Consider adding a constructor that doesn't require the message format pattern and log file path to be passed.
	 * For the log file path (@ref mLogFilePath), maybe a function should exist that automatically generates a file path
	 * for you? Or maybe a function should exist that automatically generates a log file name for you? And for the message
	 * format pattern, the one that is provided in the Spdlog documentation could be used as a default value.
	 */
	class SpdlogLogger : public ALogger {
	public:
		/**
		 * @brief The standard spdlog console color sink.
		 * 
		 * @note This is just to make the code more readable
		 * 
		 * @since v0.0.1
		 */
		using ConsoleColorSink = spdlog::sinks::stdout_color_sink_mt;

		/**
		 * @brief The standard spdlog basic file sink.
		 * 
		 * @note This is just to make the code more readable
		 * 
		 * @since v0.0.1
		 */
		using BasicFileSink = spdlog::sinks::basic_file_sink_mt;

		/**
		* @brief A struct that bundles together the console sink and the file sink as it is required for the spdlog logger
		 * during it's initialization.
		 * 
		 * @since v0.0.1
		 */
		struct LoggerSinks {

			/**
			 * @brief The console color sink that will be used by the spdlog logger. Unless you are doing something that
			 * requires a special console color sink, you should use the standard spdlog console color sink that is already
			 * being used.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref ConsoleColorSink
			 */
			std::shared_ptr<ConsoleColorSink> consoleSink;
			
			/**
			 * @brief The basic file sink that will be used by the spdlog logger. When a message is passed to this sink, it
			 * will be written to the log file at the path specified in the constructor of @a SpdlogLogger.
			 * 
			 * @since v0.0.1
			 * 
			 * @sa @ref BasicFileSink
			 */
			std::shared_ptr<BasicFileSink> fileSink;

			LoggerSinks(
				std::shared_ptr<ConsoleColorSink> consoleSink,
				std::shared_ptr<BasicFileSink> fileSink) 
				: consoleSink(consoleSink), fileSink(fileSink) {
			}

			LoggerSinks() = default;
		};
	private:

		constexpr static bool SHOULD_TRUNCATE_FILE_LOGS = false;

		constexpr static ELogLevel DEFAULT_CONSOLE_SINK_LEVEL = ELogLevel::info;
		constexpr static ELogLevel DEFAULT_FILE_SINK_LEVEL = ELogLevel::trace;
		constexpr static ELogLevel DEFAULT_LOGGER_LEVEL = ELogLevel::debug;


		/**
		 * @brief A flag that indicates whether or not the log messages should be truncated if they are too long. This is used
		 * by the file sink when writing to the log file. If the message is too long, it will be truncated. By default, this
		 * flag is set to false.
		 * 
		 * @important If you want messages to be truncated, this flag should be set to true @b BEFORE calling @a init(). On a
		 * public scope, this should be done by the @c SpdlogLogger constructor that accepts a @c truncateMessages parameter.
		 * 
		 * @since v0.0.1
		 */
		bool mbTruncateMessages = SHOULD_TRUNCATE_FILE_LOGS;
		
		/**
		 * @brief A flag that indicates whether or not the logger is initialized and can be used without error.
		 * 
		 * @since v0.0.1
		 */
		bool mbIsFullyInitialized = false;

		bool mbUseFileSink = false;

		/**
		 * @brief The minimum log level for a message to be logged to the console. By default, this is set to @a warn. You can
		 * change this value by calling @a setConsoleLogLevel(). However, this is @b not recommended to be done @b after
		 * initializing the logger because you would have to shutdown the logger and reinitialize it for it to take effect.
		 * 
		 * @since v0.0.1
		 */
		ELogLevel mConsoleLogLevel = DEFAULT_CONSOLE_SINK_LEVEL;

		/**
		 * @brief The minimum log level for a message to be logged (written) to the log file. By default, this is set to @a trace. 
		 * You can change this value by calling @a setFileLogLevel(). However, this is @b not recommended to be done @b after
		 * initializing the logger because you would have to shutdown the logger and reinitialize it for it to take effect.
		 * 
		 * @since v0.0.1
		 */
		ELogLevel mFileLogLevel = DEFAULT_FILE_SINK_LEVEL;

		/**
		 * @brief The minimum log level for a message to be logged. By default, this is set to @a debug. You can change this
		 * value by calling @a setLevel(). However, this is @b not recommended to be done @b after initializing the logger
		 * because you would have to shutdown the logger and reinitialize it for it to take effect.
		 * 
		 * @since v0.0.1
		 */
		ELogLevel mLoggerLevel = DEFAULT_LOGGER_LEVEL;

		/**
		 * @brief The name of the logger that will be passed to the spdlog logger library when it is initialized. There is not
		 * much need to change this value unless you are doing something special. By default, this is set to "Atlas".
		 * 
		 * @since v0.0.1
		 */
		const std::string mLoggerName = "Atlas";

		/**
		 * @brief The path to the log file that will be written to when a message is successfully passed to the file sink.
		 * 
		 * @since v0.0.1
		 * 
		 * @todo Implement this with a @a std::filesystem::path variable.
		 */
		const std::string mLogFilePath;

		/**
		 * @brief The pattern that will be used to format the messages that are passed to the logger. The value of this should be
		 * set PRIOR to calling @a init(). Furthermore, please use the pattern flags that Spdlog provides (see this variable's
		 * see also section in the documentation for a link to the Spdlog documentation that explains the pattern flags).
		 * 
		 * @since v0.0.1
		 * 
		 * @sa https://github.com/gabime/spdlog/wiki/Custom-formatting Spdlog documentation on custom formatting. Please use
		 * the same pattern flags that Spdlog provides to format the messages that are passed to the logger.
		 */
		std::string mMessageFormatPattern;

		/**
		 * @brief A unique pointer to the @a LoggerSinks aggregator struct.
		 * 
		 * @since v0.0.1
		 */
		std::unique_ptr<LoggerSinks> mLoggingSinks = nullptr;

		/**
		 * @brief A unique pointer to the @a spdlog logger instance. All logging is done through this instance, so this must
		 * be initialized before any logging can be done.
		 * 
		 * @since v0.0.1
		 */
		std::unique_ptr<spdlog::logger> mInternalSpdlogLoggerPtr = nullptr;

		std::mutex mInternalSpdlogLoggerMutex;

		// This may or may not be used in the future.
		static ELogLevel ExtractLogLevel(std::initializer_list<ELogLevel> logLevels, int index);


		// These two functions do nothing right now, hence they are private and empty.
		void display(std::string const& message) override;
		void setLevel(ELogLevel logLevel) override;

		/**
		 * @brief Initializes the console sink in the @c LoggerSinks struct that will be used by the spdlog logger. This function
		 * will set the console sink's log level to @c mConsoleLogLevel and will set the sink's pattern to @a mMessageFormatPattern.
		 * This is called by the @c init() function. 
		 * 
		 * @note Just because this function has been called, does not mean that the console sink is ready to be used because spdlog
		 * requires the console sink to be initialized before the @c spdlog::logger instance can be initialized. In the case of this
		 * class, the @c spdlog::logger instance is stored as @ref mInternalSpdlogLoggerPtr. In other words, the variable
		 * @ref mInternalSpdlogLoggerPtr must be initialized before any logging can be done.
		 * 
		 * @since v0.0.1
		 * 
		 * @todo Implement some checks to make sure that the console sink is not already initialized and that the pattern is not empty.
		 * Doing so should make the initialization of the console sink more robust and less error-prone.
		 */
		void initConsoleSink();

		/**
		 * @brief Initializes the file sink in the @a LoggerSinks struct that will be used by the spdlog logger. This function will set
		 * the file sink's log level to the value of @a mFileLogLevel.  
		 * 
		 * @note Just because this function has been called, does not mean that the file sink is ready to be used because spdlog
		 * requires the file sink to be initialized before the @c spdlog::logger instance can be initialized.
		 * 
		 * @since v0.0.1
		 */
		void initFileSink();

		/**
		 * @brief Initializes the @a spdlog logger instance. Once this function is called (assuming that everything went okay up to this
		 * point), the @a spdlog logger instance can be used to log messages.
		 * 
		 * @pre The console sink and the file sink @b must be initialized @b PRIOR to calling this function.
		 * 
		 * @since v0.0.1
		 */
		void initInternalSpdlogLogger();

	public:

		/**
		 * @brief An explicit and verbose constructor for the @a SpdlogLogger class.
		 * 
		 * @note If the value of @c logFilePath is empty, the file sink will not be initialized. If @c logFilePath is not a valid path, an
		 * exception will be thrown when @c init() is called.
		 * 
		 * @param loggerName What the logger will be called.
		 * 
		 * @param messageFormatPattern The pattern that will be used to format the messages that are passed to the logger.
		 * 
		 * @param logFilePath The path to the log file.
		 * 
		 * @param truncateMessages Whether or not to truncate the messages that are passed to the logger.
		 * 
		 * @param sinksLogLevels An initializer list of log levels that will be used to set the log level for each sink. Unless you are
		 * implementing more than the default sinks, this should have 3 entries in accordance with the @c ESpdlogInitListIndex enum class.
		 * 
		 * @since v0.0.1
		 */
		explicit SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages,
			std::initializer_list<ELogLevel> sinksLogLevels);
			
		/**
		 * @brief An explicit constructor for the @a SpdlogLogger class that will also set the value of @c mTruncateMessages.
		 * 
		 * @note If the value of @c logFilePath is empty, the file sink will not be initialized. If @c logFilePath is not a valid path, an
		 * exception will be thrown when @c init() is called. 
		 * 
		 * @param loggerName What the logger will be called.
		 * 
		 * @param messageFormatPattern The pattern that will be used to format the messages that are passed to the logger.
		 * 
		 * @param logFilePath The path to the log file.
		 * 
		 * @param truncateMessages Whether or not to truncate the messages that are passed to the logger.
		 * 
		 * @since v0.0.1
		 */
		explicit SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages);

		/**
		 * @brief An explicit constructor for the @a SpdlogLogger class. This constructor will be suitable for most use cases.
		 * 
		 * @note If the value of @c logFilePath is empty, the file sink will not be initialized. If @c logFilePath is not a valid path, an
		 * exception will be thrown when @c init() is called.
		 * 
		 * @param loggerName What the logger will be called.
		 * 
		 * @param messageFormatPattern The pattern that will be used to format the messages that are passed to the logger.
		 * 
		 * @param logFilePath The path to the log file.
		 * 
		 * @since v0.0.1
		 */
		explicit SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath);

		SpdlogLogger() = default;

		static void QueueMessage(std::string const& message, ELogLevel logLevel);

		/**
		 * @brief Sets the logger that will be used as the default logger for @c spdlog::log() or the other namespace-scope logging functions for
		 * @c spdlog.
		 * 
		 * @param logger
		 * 
		 * @since v
		 */
		static void SetDefaultLogger(SpdlogLogger* logger);

		/**
		 * @brief Initializes the logger and the spdlog library. This function must be called before the logger can be used. Once this function
		 * is called, this logger will be able to log messages. 
		 * 
		 * @pre The required user-defined initialization variables @c mLoggerName, @c mMessageFormatPattern, and @c mLogFilePath must be set
		 * before calling this function. If it is not, behavior is undefined and exceptions may be thrown depending on what is not set.
		 * 
		 * @since v0.0.1
		 */
		void init() override;

		/**
		 * @brief Pretty much the same as @c setThisAsDefaultLogger(), but sets this logger instance as the default logger. 
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref SetDefaultLogger
		 */
		void setThisAsDefaultLogger();

		/**
		 * @brief A more explicit logging function where the message and log level must be specified. This function will only log using 
		 * @b this logger instance, not the default logger. This function is inherited from @c ALogger.
		 * 
		 * @param message A const reference to the message that will be logged.
		 * 
		 * @param logLevel The log level of the message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void log(std::string const& message, ELogLevel logLevel) override;

		/**
		 * @brief Logs a message at the @c ELogLevel::info log level using this class' @c log() function.
		 * 
		 * @param message The message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void info(std::string const& message);

		/**
		 * @brief Logs a message at the @c ELogLevel::err log level using this class' @c log() function.
		 * 
		 * @param message The message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void error(std::string const& message);

		/**
		 * @brief Logs a message at the @c ELogLevel::warn log level using this class' @c log() function.
		 * 
		 * @param message The message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void warn(std::string const& message);

		/**
		 * @brief Logs a message at the @c ELogLevel::debug log level using this class' @c log() function.
		 * 
		 * @param message The message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void debug(std::string const& message);

		/**
		 * @brief Logs a message at the @c ELogLevel::trace log level using this class' @c log() function.
		 * 
		 * @param message The message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void trace(std::string const& message);

		/**
		 * @brief Logs a message at the @c ELogLevel::critical log level using this class' @c log() function.
		 * 
		 * @param message The message that will be logged.
		 * 
		 * @since v0.0.1
		 */
		void critical(std::string const& message);

		/**
		 * @brief Closes the logger. Currently, this function does nothing.
		 * 
		 * @since v0.0.1
		 */
		void close() override;

		/**
		 * @brief Sets the console sink's log level.
		 * 
		 * @param logLevel The minimum log level that the console sink will log.
		 * 
		 * @since v0.0.1
		 */
		void setConsoleLogLevel(ELogLevel logLevel) {
			mConsoleLogLevel = logLevel;
		}

		/**
		 * @brief Sets the file sink's log level.
		 * 
		 * @param logLevel The minimum log level that the file sink will log.
		 * 
		 * @since v0.0.1
		 */
		void setFileLogLevel(ELogLevel logLevel) {
			mFileLogLevel = logLevel;
		}

		/**
		 * @brief Sets the logger's log level.
		 * 
		 * @param logLevel The minimum log level that the logger will log.
		 * 
		 * @since v0.0.1
		 */
		void setLoggerLevel(ELogLevel logLevel) {
			mLoggerLevel = logLevel; 
		}
		
		/**
		 * @brief Returns whether or not the logger should truncate the message, derived from the @c mbTruncateMessages flag.
		 * 
		 * @return @c true if the logger should truncate the message, @c false otherwise.
		 * 
		 * @since v0.0.1
		 */
		bool shouldTruncateMessage() const;
	};

	void LogMessage(std::string const& message, ELogLevel logLevel);

	void InfoLog(std::string const& message);

	void ErrorLog(std::string const& message);

	void WarnLog(std::string const& message);

	void DebugLog(std::string const& message);

	void TraceLog(std::string const& message);

	void CriticalLog(std::string const& message);

#ifdef ATLAS_USE_SPDLOG
	//SpdlogLogLevel TranslateAtlasLogLevelToSpdlog(ELogLevel atlasLogLevel);

#endif

	//class Logger : public SpdlogLogger {
	//public:
	//	using SpdlogLogger::SpdlogLogger;
	//	//Logger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath);
	//	Logger() = default;
	//};
}
