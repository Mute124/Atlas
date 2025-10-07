#include <cstdint>
#include <string>

#include "Logging.h"
#include <spdlog/logger.h>
#include "../core/MemoryAllocator.h"
#include <memory>

void Atlas::SpdlogLogger::display(std::string const& message)
{
}

Atlas::SpdlogLogger::SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, LoggerSinks* loggingSinks)
	: mLoggerName(loggerName), mLogFilePath(logFilePath), mLoggingSinks(loggingSinks), mMessageFormatPattern(messageFormatPattern)
{
}

void Atlas::SpdlogLogger::init(LoggerSinks* loggingSinks)
{
	this->mInternalSpdlogLogger = std::make_shared<spdlog::logger>(
		spdlog::logger(mLoggerName, { loggingSinks->consoleSink, loggingSinks->fileSink }));

	this->mInternalSpdlogLogger->set_pattern(mMessageFormatPattern);
}

void Atlas::SpdlogLogger::init()
{
	this->init(this->mLoggingSinks);
}

void Atlas::SpdlogLogger::log(std::string const& message, uint16_t logLevel)
{
}

void Atlas::SpdlogLogger::close()
{
}

void Atlas::SpdlogLogger::setLevel(uint16_t logLevel)
{
}
