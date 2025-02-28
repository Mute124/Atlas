#pragma once
#include "ELoggingMode.h"
#include "ELogLevel.h"

#include <memory>
#include <source_location>
#include <string>
#include <vadefs.h>
#include <vector>
#include <queue>
#include <mutex>

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "../utils/Singleton.h"

#define ATLAS_DEFAULT_LOG_LEVEL Atlas::ELogLevel::TRACE


namespace Atlas {	

	/// <summary>
	/// This is NOT what will print to the terminal, rather this is an interface where the logger will pass logging messages to. Use this for in-game consoles or something like that.
	/// </summary>
	
	/**
	 * @brief This is a sink for message logging. 
	 * @remarks This is used when the @ref Logger class recieves logs. Afterwards, it is forwarded to the @ref IConsoleSink. 
	 * @note This can be used for in-game consoles or something like that.
	 * @note This is @b NOT what will print to the terminal, rather this is an interface where the logger will pass logging messages to
	 * @since v0.0.3
	 */
	class IConsoleSink {
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
		ELogLevel loggerLevel = ELogLevel::TRACE;
		
		std::string format = "[Atlas] [%^%l%$] %v";
		std::string logFile =  CreateLogFileName();
	};


	class Logger : public Singleton<Logger> {
	private:
		struct TemporaryMessage {
			std::string message;
			ELogLevel level;
			std::source_location location;

			TemporaryMessage(const std::string& message, const ELogLevel& level, const std::source_location& location)
				: message(message), level(level), location(location)
			{
			}

			TemporaryMessage() = default;
		};

		class MessageBuffer {
		private:
			std::queue<TemporaryMessage> mBuffer;
		public:
			void push(TemporaryMessage message) { 
				mBuffer.push(message); 
			}

			TemporaryMessage pop() { 
				TemporaryMessage temp = mBuffer.front();
				mBuffer.pop();
				return temp; 
			}

			bool isEmpty() { return mBuffer.empty(); }
		};

		LoggerConfig mConfig;
		std::shared_ptr<spdlog::logger> mLogger;

		// 

		/**
		 * @brief This is a buffer of logs that will be printed when the logger is initialized.
		 * @remarks This is used when the logger is used but is not initialized. This is also a ptr because it will be deleted after the logs are printed.
		 * @since v0.0.9
		 */

		void InitSpdlog();
	
	public:
		~Logger();
		void init(LoggerConfig config);

		void log(std::string message, ELogLevel level = ELogLevel::DEBUG, const std::source_location& location = std::source_location::current());

		static inline void RaylibLogCallback(int logLevel, const char* message, va_list args);

	};
	
	spdlog::level::level_enum GetSpdlogLevel(ELogLevel level);
	void Log(std::string message, ELogLevel level = ELogLevel::DEBUG, const std::source_location& location = std::source_location::current());
}