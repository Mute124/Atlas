/**************************************************************************************************
 * @file Logging.h
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include <string>
#include <cstdint>
#include <memory>


#include "../core/Common.h"
#include "../core/MemoryAllocator.h"


#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/common.h>


namespace Atlas {

	using LogLevel = spdlog::level::level_enum;

	/**
	 * @brief Interface for logging that is responsible for handling any logging from the engine, game, or mods.
	 *
	 * @since v0.0.1
	 */
	class ILogger {
	private:
		//static inline AbstractMemoryAllocator* sMemoryAllocator = nullptr;

	public:
		
		virtual ~ILogger() = default;

		virtual void init() = 0;

		virtual void close() = 0;

		virtual void display(std::string const& message) = 0;

		virtual void log(std::string const& message, uint16_t logLevel) = 0;
	
		virtual void setLevel(uint16_t logLevel) = 0;
	};

	/**
	 * @brief A generic logger that uses the @b Spdlog library. According to the spdlog documentation, you may call the
	 * @a spdlog::log() function to log messages @b after the logger has been initialized because of how spdlog works.
	 * For more information, on how to use spdlog, please refer to their documentation. 
	 * 
	 * @important While most of the variables that are required for initialization within this class, there are a couple
	 * that must absolutely be set because there is no default value for them. More specifically, the values of the
	 * following variables that do not have a default value must be set PRIOR to calling @ref init():
	 *
	 * - @ref mMessageFormatPattern
	 * - @ref mLogFilePath
	 *
	 *
	 * @since v0.0.1
	 * 
	 * @sa @ref ILogger The interface that this class implements
	 */
	class SpdlogLogger : public ILogger {
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
		
		/**
		 * @brief A flag that indicates whether or not the log messages should be truncated if they are too long. This is used
		 * by the file sink when writing to the log file. If the message is too long, it will be truncated. By default, this
		 * flag is set to false.
		 * 
		 * @important If you want messages to be truncated, this flag should be set to true @b BEFORE calling @a init(). On a
		 * public scope, this should be done by the @a SpdlogLogger constructor that accepts a @a truncateMessages parameter.
		 * 
		 * @since v0.0.1
		 */
		bool mbTruncateMessages = false;
		
		/**
		 * @brief A flag that indicates whether or not the logger is initialized and can be used without error.
		 * 
		 * @since v0.0.1
		 */
		bool mbIsInitialized = false;

		/**
		 * @brief The minimum log level for a message to be logged to the console. By default, this is set to @a warn. You can
		 * change this value by calling @a setConsoleLogLevel(). However, this is @b not recommended to be done @b after
		 * initializing the logger because you would have to shutdown the logger and reinitialize it for it to take effect.
		 * 
		 * @since v0.0.1
		 */
		LogLevel mConsoleLogLevel = LogLevel::warn;

		/**
		 * @brief The minimum log level for a message to be logged (written) to the log file. By default, this is set to @a trace. 
		 * You can change this value by calling @a setFileLogLevel(). However, this is @b not recommended to be done @b after
		 * initializing the logger because you would have to shutdown the logger and reinitialize it for it to take effect.
		 * 
		 * @since v0.0.1
		 */
		LogLevel mFileLogLevel = LogLevel::trace;

		/**
		 * @brief The minimum log level for a message to be logged. By default, this is set to @a debug. You can change this
		 * value by calling @a setLevel(). However, this is @b not recommended to be done @b after initializing the logger
		 * because you would have to shutdown the logger and reinitialize it for it to take effect.
		 * 
		 * @since v0.0.1
		 */
		LogLevel mLoggerLevel = LogLevel::debug;

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
		 * @since v
		 */
		const std::string mLogFilePath;

		std::string mMessageFormatPattern;

		std::unique_ptr<LoggerSinks> mLoggingSinks;
		std::unique_ptr<spdlog::logger> mInternalSpdlogLoggerPtr;

		void display(std::string const& message) override;
		
		void initConsoleSink();

		void initFileSink();

		void initInternalSpdlogLogger();

	public:

		explicit SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages);

		explicit SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath);

		SpdlogLogger() = default;

		void init() override;

		void log(std::string const& message, uint16_t logLevel) override;

		void info(std::string const& message);

		void error(std::string const& message);

		void warn(std::string const& message);

		void debug(std::string const& message);

		void trace(std::string const& message);

		void critical(std::string const& message);

		void close() override;

		void setConsoleLogLevel(LogLevel logLevel) {
			mConsoleLogLevel = logLevel;
		}

		void setFileLogLevel(LogLevel logLevel) { 
			mFileLogLevel = logLevel;
		}

		void setLoggerLevel(LogLevel logLevel) { 
			mLoggerLevel = logLevel; 
		}

		// Inherited via ILogger
		void setLevel(uint16_t logLevel) override;
		
		bool shouldTruncateMessage() const { return mbTruncateMessages; }

	};


	class Logger : public SpdlogLogger {
	public:
		using SpdlogLogger::SpdlogLogger;
		//Logger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath);
		Logger() = default;
	};

}
