#include <cstdint>
#include <string>
#include <initializer_list>
#include <memory>
#include <chrono>
#include <ctime>
#include <format>
#include <source_location>
#include <stdexcept>
#include <filesystem>

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../core/MemoryAllocator.h"
#include "Logging.h"
#include "../core/Common.h"



//void Atlas::ALogger::QueuedMessagesContainer::push(LogMessage const& message) {
//	std::scoped_lock lock(messagesMutex);
//	messages.push(message);
//}
//
//void Atlas::ALogger::QueuedMessagesContainer::clear() {
//	std::scoped_lock lock(messagesMutex);
//	messages = std::queue<LogMessage>();
//}
//
//Atlas::LogMessage Atlas::ALogger::QueuedMessagesContainer::pop() {
//	std::scoped_lock lock(messagesMutex);
//	LogMessage message = messages.front();
//	messages.pop();
//
//	return message;
//}
//
//bool Atlas::ALogger::QueuedMessagesContainer::empty() const {
//	//std::scoped_lock lock(messagesMutex);
//	return messages.empty();
//}

std::string Atlas::ALogger::GenerateLogFileName() {
	// Generate a log file name from the current date and time.

	const auto now = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
	std::string fileName = std::format("{:%Y-%m-%d_%H-%M-%S}.log", now);

	return fileName;
}

void Atlas::ALogger::Log(std::string const& message, ELogLevel logLevel)
{
	//std::scoped_lock lock(sLogger->mMutex);
	//sLogger->queueMessage(message, logLevel);
}

Atlas::ELogLevel Atlas::SpdlogLogger::ExtractLogLevel(std::initializer_list<ELogLevel> logLevels, int index) {
	if (index < 0 || index >= logLevels.size()) {
		return ELogLevel::trace;
	}

	return logLevels.begin()[index]; 
}

void Atlas::SpdlogLogger::display(std::string const& message)
{
}

void Atlas::SpdlogLogger::setLevel(ELogLevel logLevel)
{
}

void Atlas::SpdlogLogger::initConsoleSink()
{
	const spdlog::level::level_enum cCastedConsoleLogLevel = static_cast<spdlog::level::level_enum>(mConsoleLogLevel);

	mLoggingSinks->consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	mLoggingSinks->consoleSink->set_level(cCastedConsoleLogLevel);
	mLoggingSinks->consoleSink->set_pattern(mMessageFormatPattern);
	
}

void Atlas::SpdlogLogger::initFileSink()
{
	const spdlog::level::level_enum cCastedFileLogLevel = static_cast<spdlog::level::level_enum>(mFileLogLevel);

	mLoggingSinks->fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(mLogFilePath, mbTruncateMessages);
	mLoggingSinks->fileSink->set_level(cCastedFileLogLevel);
}

void Atlas::SpdlogLogger::initInternalSpdlogLogger()
{
	const spdlog::level::level_enum cCastedLoggerLevel = static_cast<spdlog::level::level_enum>(mLoggerLevel);
	
	mInternalSpdlogLoggerPtr = std::make_unique<spdlog::logger>(spdlog::logger(mLoggerName, { mLoggingSinks->consoleSink, mLoggingSinks->fileSink }));
	mInternalSpdlogLoggerPtr->set_level(cCastedLoggerLevel);
}

Atlas::SpdlogLogger::SpdlogLogger(std::string const& loggerName, std::string const& messageFormatPattern, std::string const& logFilePath, bool truncateMessages, std::initializer_list<ELogLevel> sinksLogLevels)
	: ALogger(), mbTruncateMessages(truncateMessages), mConsoleLogLevel(sinksLogLevels.begin()[(int)ESpdlogInitListIndex::ConsoleLogger]),
	mFileLogLevel(sinksLogLevels.begin()[(int)ESpdlogInitListIndex::FileLogger]),
	mLoggerLevel(sinksLogLevels.begin()[(int)ESpdlogInitListIndex::Logger]), mLoggerName(loggerName), mLogFilePath(logFilePath),
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

void Atlas::SpdlogLogger::QueueMessage(std::string const& message, ELogLevel logLevel)
{
	bool bDefaultLoggerExists = spdlog::default_logger() != nullptr;

	if(bDefaultLoggerExists) {
		spdlog::default_logger()->log(static_cast<spdlog::level::level_enum>(logLevel), message);
	} else {
		
	}
}

void Atlas::SpdlogLogger::SetDefaultLogger(SpdlogLogger* logger)
{
	ATLAS_ASSERT(logger != nullptr, "A nullptr logger cannot be set as the default logger!");

	spdlog::set_default_logger(std::make_shared<spdlog::logger>(*logger->mInternalSpdlogLoggerPtr));
}

void Atlas::SpdlogLogger::init()
{
	if (mInternalSpdlogLoggerPtr != nullptr) {
		mInternalSpdlogLoggerPtr->warn("Logger already initialized!");

		//throw std::runtime_error("Logger already initialized!");

		return;
	}

	mLoggingSinks = std::make_unique<LoggerSinks>();

	initConsoleSink();
	initFileSink();
	initInternalSpdlogLogger();

	mInternalSpdlogLoggerPtr->info(
		std::format("The logger with the name of {} initialized successfully! Log file can be found at: {}", mLoggerName, std::filesystem::path(mLogFilePath).string())
	);

	mbIsFullyInitialized = true;
}

void Atlas::SpdlogLogger::setThisAsDefaultLogger() { 
	SetDefaultLogger(this); 
}

void Atlas::SpdlogLogger::log(std::string const& message, ELogLevel logLevel)
{
	mInternalSpdlogLoggerPtr->log(static_cast<spdlog::level::level_enum>(logLevel), message);
}

void Atlas::SpdlogLogger::info(std::string const& message)
{
	log(message, ELogLevel::info);
}

void Atlas::SpdlogLogger::error(std::string const& message)
{
	log(message, ELogLevel::error);
}

void Atlas::SpdlogLogger::warn(std::string const& message)
{
	log(message, ELogLevel::warn);
}

void Atlas::SpdlogLogger::debug(std::string const& message)
{
	log(message, ELogLevel::debug);
}

void Atlas::SpdlogLogger::trace(std::string const& message)
{
	log(message, ELogLevel::trace);
}

void Atlas::SpdlogLogger::critical(std::string const& message)
{
	log(message, ELogLevel::critical);
}

void Atlas::SpdlogLogger::close()
{
	mbIsFullyInitialized = false;
}

bool Atlas::SpdlogLogger::shouldTruncateMessage() const { 
	return mbTruncateMessages;
}

void Atlas::LogMessage(std::string const& message, ELogLevel logLevel)
{
	spdlog::log(static_cast<spdlog::level::level_enum>(logLevel), message);
}

void Atlas::InfoLog(std::string const& message)
{
	LogMessage(message, ELogLevel::info);
}

void Atlas::ErrorLog(std::string const& message)
{
	LogMessage(message, ELogLevel::error);
}

void Atlas::WarnLog(std::string const& message)
{
	LogMessage(message, ELogLevel::warn);
}

void Atlas::DebugLog(std::string const& message)
{
	LogMessage(message, ELogLevel::debug);
}

void Atlas::TraceLog(std::string const& message)
{
	LogMessage(message, ELogLevel::trace);
}

void Atlas::CriticalLog(std::string const& message)
{
	LogMessage(message, ELogLevel::critical);
}

