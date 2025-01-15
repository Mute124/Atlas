#include "Console.h"
std::string dummyPrompt = "> ";
void Atlas::Console::parseInput(const std::string& input)
{
}

std::string& Atlas::Console::getPrompt()
{
    // TODO: insert return statement here
    return dummyPrompt;
}

void Atlas::Console::draw()
{
}

void Atlas::ConsoleSink::parseLog(const std::string& message, ELogLevel level, const std::source_location& location)
{
    // if the buffer has reached the limit, remove the oldest message
    if (mBuffer.size() >= mLimit) {
		mBuffer.erase(mBuffer.begin(), mBuffer.begin() + 1);
	}
    // if the log level is only to be shown to a file, don't print to the console
    if (!mShowInConsole && (level == ELogLevel::INFO || level == ELogLevel::WARNING || level == ELogLevel::ERROR || level == ELogLevel::FATAL)) {
		return;
	}

    mBuffer.push_back(message);

    
}

void Atlas::ConsoleSink::parsePrompt(const std::string& message)
{
}

std::vector<std::string>& Atlas::ConsoleSink::getBuffer()
{
    return mBuffer;
}
