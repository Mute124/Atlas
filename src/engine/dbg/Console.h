#pragma once
#include "Logging.h"

namespace Atlas {

	template<typename ConsoleSink>
	class IConsole abstract {
	public:
		explicit IConsole(ConsoleSink& sink) : mSink(sink) {}

		virtual void parseInput(const std::string& input) = 0;
		
		virtual std::string& getPrompt() = 0;

		virtual void draw() = 0;
		virtual void update() = 0;
		virtual void clear() = 0;
	protected:
		ConsoleSink& mSink;
	};
	
	class ConsoleSink : public IConsoleSink {
	public:


		// Inherited via IConsoleSink
		void parseLog(const std::string& message, ELogLevel level, const std::source_location& location) override;

		void parsePrompt(const std::string& message) override;

		std::vector<std::string>& getBuffer() override;

	private:
		std::string mHistory;
		bool mHistoryIsDirty = false;
		bool mShowInConsole = true;
		int mLimit = 100;

		std::vector<std::string> mBuffer;
	};

	class Console : public IConsole<ConsoleSink> {
	public:


		// Inherited via IConsole
		void parseInput(const std::string& input) override;

		std::string& getPrompt() override;

		void draw() override;

	};
}