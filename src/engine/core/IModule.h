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

	/**
	 * @brief This is the base class for all modules within Atlas.
	 * 
	 * @since v0.0.9
	 */
	class IModule {
	public:
		class ModuleAttributes {
		private:
			const std::string cName;
			
			const std::string cCategory;
			
			const bool cEnabled;

		public:

			ModuleAttributes();

			explicit ModuleAttributes(std::string const& name, std::string const& category);

			/**
			 * @brief The recomended constructor.
			 * 
			 * @param name
			 * @param category
			 * @param enabled
			 * 
			 * @since 
			 */
			explicit ModuleAttributes(std::string const& name, std::string const& category, bool enabled);

			std::string const& getName() const;

			std::string const& getCategory() const;

			bool isEnabled() const;
		};

		ModuleAttributes attributes;

		virtual void preInit() = 0;

		virtual void init() = 0;

		virtual void postInit() = 0;
	};
}
