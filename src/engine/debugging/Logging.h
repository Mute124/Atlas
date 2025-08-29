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
	 * @brief A generic logger that uses the @b Spdlog library.
	 *
	 * @since v0.0.1
	 */
	class SpdlogLogger : public ILogger {
	public:
		struct LoggerSinks {
			std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
			std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink;
		};

	private:

		const std::string mLoggerName;
		const std::string mLogFilePath;		

		LoggerSinks* mLoggingSinks;

		std::string mMessageFormatPattern;

		std::shared_ptr<spdlog::logger> mInternalSpdlogLogger;

		void display(std::string const& message) override;
		
	public:

		SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, LoggerSinks* loggingSinks)
			: mLoggerName(loggerName), mLogFilePath(logFilePath), mLoggingSinks(loggingSinks), mMessageFormatPattern(messageFormatPattern)
		{
		}

		void init(LoggerSinks* loggingSinks)
		{
			this->mInternalSpdlogLogger = std::make_shared<spdlog::logger>(
				spdlog::logger(mLoggerName, { loggingSinks->consoleSink, loggingSinks->fileSink }));

			this->mInternalSpdlogLogger->set_pattern(mMessageFormatPattern);
		}
		void init() override
		{
			this->init(this->mLoggingSinks);
		}

		void log(std::string const& message, uint16_t logLevel) override;
		

		void close() override;

		// Inherited via ILogger
		void setLevel(uint16_t logLevel) override
		{
		}
	};


	class Logger : public SpdlogLogger {
	public:
		using SpdlogLogger::SpdlogLogger;
		//Logger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath);
		Logger() = default;
	};

}
