#include "Logging.h"
#include "../core/Common.h"
#include "spdlog\common.h"
#include "spdlog\logger.h"
#include "spdlog\sinks\basic_file_sink.h"
#include "spdlog\sinks\stdout_color_sinks.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "ELogLevel.h"
#include "corecrt.h"
#include "stdio.h"
#include "time.h"
#include <cstdarg>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <source_location>
#include <stdexcept>
#include <string>
#include "vadefs.h"
#include "../Project.h"

void Atlas::TerminalSink::init()
{
    mColorSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    mColorSink->set_level(spdlog::level::warn);
    mColorSink->set_pattern(mFormat);
}

std::string Atlas::TerminalSink::getFormat()
{
    return this->mFormat;
}

std::string Atlas::CreateLogFileName() {
    // Filename should follow this format: YYYY-MM-DD_HH-MM-SS.log
    time_t now = time(0);
    struct tm tstruct;
    localtime_s(&tstruct, &now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);


    std::string name = ATLAS_TEMP_DIR;
	name += "logs/";
	name += std::string(buf);
	name += ".log";

    return name;
}

spdlog::level::level_enum Atlas::GetSpdlogLevel(ELogLevel level) {
    return static_cast<spdlog::level::level_enum>(level);
}

void Atlas::Log(std::string message, ELogLevel level, const std::source_location& location) {
	GetAtlasEngine()->getLogger().get()->log(message, level, location);
	//Logger::Instance().log(message, level, location);
}

void Atlas::Logger::InitSpdlog()
{
/*	spdlog::init_thread_pool(8192, 1);

	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();

	auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		"mylog.txt",
		1024 * 1024 * 10,
		3
	);
	
	std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
	
	auto logger = std::make_shared<spdlog::async_logger>(
		"loggername",
		sinks.begin(),
		sinks.end(),
		spdlog::thread_pool(),
		spdlog::async_overflow_policy::block
	);*/

	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	if(console_sink == nullptr) {
		throw std::runtime_error("Failed to create console sink");
	}

	console_sink->set_level(GetSpdlogLevel(mConfig.terminalLogLevel));
	console_sink->set_pattern(mConfig.format);

	std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(mConfig.logFile, true);

	if(file_sink == nullptr) {
		throw std::runtime_error("Failed to create file sink");
	}

	file_sink->set_level(GetSpdlogLevel(mConfig.fileLogLevel));

	std::string loggerName = typeid(*this).name();

	// shorten down the name to just the typename. In this case, "Atlas::Logger"
	loggerName = loggerName.substr(loggerName.find_last_of("::") + 1);

	// remove anything to the right of the typename, so it looks like "Atlas::Logger"
	loggerName = loggerName.substr(0, loggerName.find_last_of("::"));

	
	mLogger = std::make_shared<spdlog::logger>(spdlog::logger("Atlas", {console_sink, file_sink}));

	if(mLogger == nullptr) {
		throw std::runtime_error("Failed to create logger");
	}

	mLogger->set_level(GetSpdlogLevel(mConfig.loggerLevel));

	mLogger->trace("Test");
	


	//SetTraceLogCallback(&RaylibLogCallback);
}

Atlas::Logger::~Logger()
{
	mLogger->trace("Test2");

}

void Atlas::Logger::init(LoggerConfig config) {
    this->mConfig = config;
    InitSpdlog();
    log("Logger initialized");
}

void Atlas::Logger::log(std::string message, ELogLevel level, const std::source_location& location)
{
	
	if(mLogger == nullptr)
	{
/*		TemporaryMessage failureWarningMessage("Failed to log message because the logger was not initialized. Putting message into a buffer.", ELogLevel::WARNING, location);
		TemporaryMessage temporaryMessage(message, level, location);
		//std::lock_guard<std::mutex> lock(mBufferMutex);

		if(mBuffer == nullptr)
		{
			mBuffer = std::make_shared<MessageBuffer>();
		}

		mBuffer->push(failureWarningMessage);
		mBuffer->push(temporaryMessage);*/

		std::cout << "Failed: " << std::this_thread::get_id() << std::endl;

		return;
	}

	std::string locFileName = location.file_name();

	// shorten the filename to only the last part of it
	locFileName = locFileName.substr(locFileName.find_last_of("/\\") + 1);


	int locLine = location.line();

	std::string finalMessage = "[" + locFileName + ":" + std::to_string(locLine) + "] " + message;

	try
	{

		switch(level)
		{
			case Atlas::ELogLevel::TRACE:
				mLogger->trace(finalMessage);
				break;
			case Atlas::ELogLevel::DEBUG:
				mLogger->debug(finalMessage);
				break;
			case Atlas::ELogLevel::INFO:
				mLogger->info(finalMessage);
				break;
			case Atlas::ELogLevel::WARNING:
				mLogger->warn(finalMessage);
				break;
			case Atlas::ELogLevel::ERROR:
				mLogger->error(finalMessage);
				break;
			case Atlas::ELogLevel::FATAL:
				mLogger->critical(finalMessage);
				break;
			case Atlas::ELogLevel::NONE:
				break;
			default:
#ifdef RELEASE
				/*std::format("Failed to log the message: {} because the log level: {} is not supported!", finalMessage, static_cast<int>(level))*/
				// if on release, log this as an error
				std::string failMsg = "Failed to log the message: " + finalMessage;
				failMsg << "Because the log level " << level << " is not supported!";
				mLogger->error(failMsg);

#else
				mLogger->trace(finalMessage);
#endif
				break;
		}

	} catch(const std::exception& e)
	{
		std::cout << "Failed to log the message: " << finalMessage << " because: " << e.what() << std::endl;
	}
}

void Atlas::Logger::RaylibLogCallback(int logLevel, const char* message, va_list args) {

	// Determine the length of the formatted string
	va_list argsCopy;
	va_copy(argsCopy, args);
	int length = vsnprintf(nullptr, 0, message, argsCopy);
	va_end(argsCopy);

	if(length < 0) {
		// Error handling if formatting fails

	}

	// Allocate a buffer of the required size (+1 for null terminator)
	std::string formattedString(length, '\0');

	// Format the string into the allocated buffer
	vsnprintf(&formattedString[0], length + 1, message, args);

	// -1 is being done here because we do not have the same number of levels as raylib.
	Log(formattedString, static_cast<ELogLevel>(logLevel - 1), std::source_location::current());
}
