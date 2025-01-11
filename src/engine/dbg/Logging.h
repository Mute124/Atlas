#pragma once
#include "../utils/Singleton.h"
#include "ELoggingMode.h"
#include "ELogLevel.h"

#include <cstdarg>
#include <memory>
#include <source_location>
#include <stdio.h>
#include <string>
#include <vadefs.h>
#include <vector>

#include <raylib.h>

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>


#define ATLAS_DEFAULT_LOG_LEVEL Atlas::ELogLevel::TRACE


namespace Atlas {	

	/// <summary>
	/// This is NOT what will print to the terminal, rather this is an interface where the logger will pass logging messages to. Use this for in-game consoles or something like that.
	/// </summary>
	class IConsoleSink abstract {
	public:
		virtual void parseLog(const std::string& message, ELogLevel level, const std::source_location& location) = 0;
		virtual void parsePrompt(const std::string& message) = 0;

		virtual std::vector<std::string>& getBuffer() = 0;
	};

	using DefaultColorSink = std::shared_ptr<spdlog::sinks::stdout_color_sink_mt>;

	template<typename ColorSinkType>
	class ITerminalSink abstract  {
	public:
		virtual void init() = 0;
		virtual std::string getFormat() = 0;

	protected:
		ColorSinkType mColorSink;
	};

	class TerminalSink : public ITerminalSink<DefaultColorSink>{
	public:
		void init() override;
		std::string getFormat() override;
		void setFormat(std::string const& format) { mFormat = format; }

	private:
		std::string mFormat = "[Atlas] [%^%l%$] %v";
	};
	
	std::string CreateLogFileName();

	struct LoggerConfig {
		ELoggingMode mode = ELoggingMode::UNKNOWN;
		ELogLevel terminalLogLevel = ELogLevel::WARNING;
		ELogLevel fileLogLevel = ELogLevel::TRACE;
		ELogLevel loggerLevel = ELogLevel::DEBUG;
		
		std::string format = "[Atlas] [%^%l%$] %v";
		std::string logFile =  CreateLogFileName();
	};

	spdlog::level::level_enum GetSpdlogLevel(ELogLevel level);
	
	class Logger : public Singleton<Logger> {
	public:

		void init(LoggerConfig config) {
			this->mConfig = config;
			InitSpdlog();
			log("Logger initialized");
		}

		void log(std::string message, ELogLevel level = ELogLevel::DEBUG, const std::source_location& location = std::source_location::current())
		{
			std::string locFileName = location.file_name();

			// shorten the filename to only the last part of it
			locFileName = locFileName.substr(locFileName.find_last_of("/\\") + 1);


			int locLine = location.line();

			std::string finalMessage = "[" + locFileName + ":" + std::to_string(locLine) + "] " + message;

			try
			{
/*
				switch (level)
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
					// if on release, log this as an error
					std::string failMsg = "Failed to log the message: " + finalMessage;
					failMsg << "Because the log level " << level << " is not supported!";
					mLogger->error(failMsg);

#else
					mLogger->trace(finalMessage);
#endif
					break;
				}
*/
			}
			catch (const std::exception& e)
			{
				//std::cout << "Failed to log the message: " << finalMessage << " because: " << e.what() << std::endl;
			}
		}

		static inline void RaylibLogCallback(int logLevel, const char* message, va_list args) {
			
			// Determine the length of the formatted string
			va_list argsCopy;
			va_copy(argsCopy, args);
			int length = vsnprintf(nullptr, 0, message, argsCopy);
			va_end(argsCopy);

			if (length < 0) {
				// Error handling if formatting fails
				
			}

			// Allocate a buffer of the required size (+1 for null terminator)
			std::string formattedString(length, '\0');

			// Format the string into the allocated buffer
			vsnprintf(&formattedString[0], length + 1, message, args);

			// -1 is being done here because we do not have the same number of levels as raylib.
			Logger::Instance().log(formattedString, static_cast<ELogLevel>(logLevel -1), std::source_location::current());
		}
	private:
		LoggerConfig mConfig;

		spdlog::logger* mLogger;



		void InitSpdlog()
		{
			std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console_sink->set_level(GetSpdlogLevel(mConfig.terminalLogLevel));
			console_sink->set_pattern(mConfig.format);



			std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(mConfig.logFile, true);
			file_sink->set_level(GetSpdlogLevel(mConfig.fileLogLevel));
			
			std::string loggerName = typeid(*this).name();

			// shorten down the name to just the typename. In this case, "Atlas::Logger"
			loggerName = loggerName.substr(loggerName.find_last_of("::") + 1);

			// remove anything to the right of the typename, so it looks like "Atlas::Logger"
			loggerName = loggerName.substr(0, loggerName.find_last_of("::"));
	

			mLogger = new spdlog::logger("multi_sink", {console_sink, file_sink});
			mLogger->set_level(GetSpdlogLevel(mConfig.loggerLevel));
			mLogger->warn("this should appear in both console and file");
			mLogger->info("this message should not appear in the console, only in the file");
			
			//SetTraceLogCallback(&RaylibLogCallback);
		}
	};

	inline void Log(std::string message, ELogLevel level = ELogLevel::DEBUG, const std::source_location& location = std::source_location::current()) {
		Logger::Instance().log(message, level, location);
	}
}