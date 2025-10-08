#include <cstdint>
#include <string>
#include <initializer_list>
#include <memory>

#include "../core/MemoryAllocator.h"
#include "Logging.h"

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

inline Atlas::LogLevel Atlas::SpdlogLogger::ExtractLogLevel(std::initializer_list<LogLevel> logLevels, int index) { 
	if (index < 0 || index >= logLevels.size()) {
		return LogLevel::trace;
	}

	return logLevels.begin()[index]; 
}

void Atlas::SpdlogLogger::display(std::string const& message)
{
}

void Atlas::SpdlogLogger::setLevel(LogLevel logLevel)
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

Atlas::SpdlogLogger::SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages, std::initializer_list<LogLevel> sinksLogLevels) 
	: mbTruncateMessages(truncateMessages), mConsoleLogLevel(sinksLogLevels.begin()[ESpdlogInitListIndex::ConsoleLogger]),
	mFileLogLevel(sinksLogLevels.begin()[ESpdlogInitListIndex::FileLogger]),
	mLoggerLevel(sinksLogLevels.begin()[ESpdlogInitListIndex::Logger]), mLoggerName(loggerName), mLogFilePath(logFilePath),
	mMessageFormatPattern(messageFormatPattern)
{
}

Atlas::SpdlogLogger::SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages)
	: SpdlogLogger(loggerName, messageFormatPattern, logFilePath, truncateMessages, { DEFAULT_CONSOLE_SINK_LEVEL, DEFAULT_FILE_SINK_LEVEL, DEFAULT_LOGGER_LEVEL })
{
}

Atlas::SpdlogLogger::SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath)
	: SpdlogLogger(loggerName, messageFormatPattern, logFilePath, SHOULD_TRUNCATE_FILE_LOGS)
	//mLoggerName(loggerName), mLogFilePath(logFilePath), mMessageFormatPattern(messageFormatPattern)
{
}

void Atlas::SpdlogLogger::SetDefaultLogger(SpdlogLogger* logger)
{
	if (logger == nullptr) {
		return;
	}

	spdlog::set_default_logger(std::make_shared<spdlog::logger>(*logger->mInternalSpdlogLoggerPtr));
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

	mInternalSpdlogLoggerPtr->warn("Logger initialized!");

	mbIsFullyInitialized = true;
}

void Atlas::SpdlogLogger::setThisAsDefaultLogger() { 
	SetDefaultLogger(this); 
}

void Atlas::SpdlogLogger::log(std::string const& message, LogLevel logLevel)
{
	mInternalSpdlogLoggerPtr->log(static_cast<spdlog::level::level_enum>(logLevel), message);
}

void Atlas::SpdlogLogger::info(std::string const& message)
{
	log(message, LogLevel::info);
}

void Atlas::SpdlogLogger::error(std::string const& message)
{
	log(message, LogLevel::err);
}

void Atlas::SpdlogLogger::warn(std::string const& message)
{
	log(message, LogLevel::warn);
}

void Atlas::SpdlogLogger::debug(std::string const& message)
{
	log(message, LogLevel::debug);
}

void Atlas::SpdlogLogger::trace(std::string const& message)
{
	log(message, LogLevel::trace);
}

void Atlas::SpdlogLogger::critical(std::string const& message)
{
	log(message, LogLevel::critical);
}

void Atlas::SpdlogLogger::close()
{
	mbIsFullyInitialized = false;
}

bool Atlas::SpdlogLogger::shouldTruncateMessage() const { 
	return mbTruncateMessages;
}