#pragma once
#include <iostream>
namespace Techstorm {
	enum class EReturnCode : int
	{
		// Successful codes
		SUCCESSFUL_OPERATION,
		SUCCESSFUL_OPERATION_WITH_WARNING,
		SUCCESSFUL_OPERATION_WITH_CAUGHT_EXCEPTION,

		// Errors
		ERROR_UNKNOWN = -1,
		ERROR_INVALID_ARGS = -2,
		ERROR_INVALID_VALUE = -3
	};
}
