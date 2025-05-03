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
#include <memory>

#include <spdlog/spdlog.h>

#include "../core/Core.h"

#include "Logging.h"

namespace Atlas {
	class IDebugger {
	public:
		/**
		 * @brief A unique pointer to the logger that the debugger is suggested to use.
		 *  
		 * @since v0.0.1
		 */
		std::unique_ptr<ILogger> logger = nullptr;
		
		virtual void init() = 0;
	};
}
