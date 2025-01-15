#pragma once
#include <cstdint>

namespace Atlas {	
	/// <summary>
	/// What level of logging to use. This will affect the output color, severity, etc. 
	/// \note This is not the same as spdlog's log level. Also, this inherits from int to make it easier to compare.
	/// </summary>
	enum class ELogLevel : int {
		TRACE = 0,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL,
		NONE
	};
}