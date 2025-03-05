/*****************************************************************//**
 * @file   IModule.h
 * 
 * @brief  
 * 
 * @date   February 2025
 * 
 * @since v0.0.9
 *********************************************************************/
#pragma once

#include <string>

#include "../Common.h"

namespace Atlas {


	//ModuleAttributes attributes;
	/**
	 * @brief This is the base class for all modules within Atlas.
	 * 
	 * @since v0.0.9
	 */
	class IModule {
	public:

		IModule(bool enabled, bool initInConstructor);

		IModule() = default;

		virtual ~IModule();

		virtual void setup() = 0;

		virtual void init() = 0;

		virtual void update() = 0;

		virtual void cleanup() = 0;
	};

}
