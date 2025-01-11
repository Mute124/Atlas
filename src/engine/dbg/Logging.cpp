#include "Logging.h"
#include "../Common.h"


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

    std::string name = std::string(ATLAS_TEMP_DIR);
	name += "logs/";
	name += std::string(buf);
	name += ".log";

    return name;
}

spdlog::level::level_enum Atlas::GetSpdlogLevel(ELogLevel level) {
    return static_cast<spdlog::level::level_enum>(level);
}

