#include <cstdint>
#include <string>

#include "Logging.h"
#include <spdlog/logger.h>
#include "../core/MemoryAllocator.h"
#include <memory>

void Atlas::SpdlogLogger::display(std::string const& message)
{
}

void Atlas::SpdlogLogger::initConsoleSink()
{
	mLoggingSinks->consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	mLoggingSinks->consoleSink->set_level(mConsoleLogLevel);
	mLoggingSinks->consoleSink->set_pattern(mMessageFormatPattern);
}

void Atlas::SpdlogLogger::initFileSink()
{
	mLoggingSinks->fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(mLogFilePath, mbTruncateMessages);
	mLoggingSinks->fileSink->set_level(mFileLogLevel);
}

void Atlas::SpdlogLogger::initInternalSpdlogLogger()
{
	mInternalSpdlogLoggerPtr = std::make_unique<spdlog::logger>(spdlog::logger(mLoggerName, { mLoggingSinks->consoleSink, mLoggingSinks->fileSink }));
	mInternalSpdlogLoggerPtr->set_level(mLoggerLevel);
}

Atlas::SpdlogLogger::SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath)
	: mLoggerName(loggerName), mLogFilePath(logFilePath), mLoggingSinks(nullptr), mMessageFormatPattern(messageFormatPattern), mInternalSpdlogLoggerPtr(nullptr)
{
}

void Atlas::SpdlogLogger::init()
{
	if (mInternalSpdlogLoggerPtr != nullptr) {
		mInternalSpdlogLoggerPtr->warn("Logger already initialized!");

		return;
	}

	mLoggingSinks = std::make_unique<LoggerSinks>();

	initConsoleSink();
	initFileSink();
	initInternalSpdlogLogger();

	mInternalSpdlogLoggerPtr->info("Logger initialized!");

	mbIsInitialized = true;
}

void Atlas::SpdlogLogger::log(std::string const& message, uint16_t logLevel)
{
	mInternalSpdlogLoggerPtr->log(static_cast<spdlog::level::level_enum>(logLevel), message);
}

void Atlas::SpdlogLogger::info(std::string const& message)
{
	mInternalSpdlogLoggerPtr->info(message);
}

void Atlas::SpdlogLogger::error(std::string const& message)
{
	mInternalSpdlogLoggerPtr->error(message);
}

void Atlas::SpdlogLogger::warn(std::string const& message)
{
	mInternalSpdlogLoggerPtr->warn(message);
}

void Atlas::SpdlogLogger::debug(std::string const& message)
{
	mInternalSpdlogLoggerPtr->debug(message);
}

void Atlas::SpdlogLogger::trace(std::string const& message)
{
	mInternalSpdlogLoggerPtr->trace(message);
}

void Atlas::SpdlogLogger::critical(std::string const& message)
{
	mInternalSpdlogLoggerPtr->critical(message);
}

void Atlas::SpdlogLogger::close()
{
	mbIsInitialized = false;
}

void Atlas::SpdlogLogger::setLevel(uint16_t logLevel)
{
}
