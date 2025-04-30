/**************************************************************************************************
 * @file Debugger.h
 * 
 * @brief Debugging tools.
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

#include <string>

#include <spdlog/spdlog.h>

#include "../core/Core.h"


namespace Atlas {
	class ILogger {
	private:
		
	public:
		virtual void init() = 0;

		virtual void display(std::string const& message) = 0;

		virtual void log(std::string const& message) = 0;
	};

	class IDebugger {
	public:


	};
}
