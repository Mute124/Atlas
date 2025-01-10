#pragma once
#include <cstdint>
namespace Atlas {
	enum class ELoggingMode : uint8_t {
		UNKNOWN = 0,
		TERMINAL_ONLY,
		CONSOLE_ONLY,
		FILE_ONLY,

		FILE_AND_TERMINAL,
		FILE_AND_CONSOLE,

		CONSOLE_AND_TERMINAL,

		ALL
	};
}