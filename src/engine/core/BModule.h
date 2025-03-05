/*****************************************************************//**
 * @file   BModule.h
 * 
 * @brief  
 * 
 * @todo Document
 * 
 * @date   March 2025
 * 
 * @since v0.0.9
 *********************************************************************/
#pragma once
#include <string>
#include "IModule.h"

#ifndef ATLAS_BARE_BONES

/**
 * @brief .
 * 
 * @since 
 */
#define ATLAS_CALL_INIT_IN_CONSTRUCTOR false

/**
 * @brief .
 * 
 * @since 
 */
#define ATLAS_DEFAULT_MODULE_ENABLED false

#endif

namespace Atlas {

#ifndef ATLAS_BARE_BONES

	/**
	 * @brief .
	 * 
	 * @since 
	 */
	class BModule : public IModule {
	public:
		/**
		 * @brief .
		 * 
		 * @since 
		 */
		class ModuleAttributes {
		private:
			/**
			 * @brief .
			 * 
			 * @since 
			 */
			const std::string cName;

			/**
			 * @brief .
			 * 
			 * @since 
			 */
			const std::string cCategory;

			/**
			 * @brief .
			 * 
			 * @since 
			 */
			const bool cEnabled;

		public:

			/**
			 * @brief .
			 * 
			 * @since 
			 */
			ModuleAttributes();

			/**
			 * @brief .
			 * 
			 * @param name
			 * @param category
			 * 
			 * 
			 * @since 
			 */
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

			/**
			 * @brief .
			 * 
			 * 
			 * @return 
			 * 
			 * @since 
			 */
			std::string const& getName() const;

			/**
			 * @brief .
			 * 
			 * 
			 * @return 
			 * 
			 * @since 
			 */
			std::string const& getCategory() const;

			/**
			 * @brief .
			 * 
			 * 
			 * @return 
			 * 
			 * @since 
			 */
			bool isEnabled() const;
		};

	private:

		/**
		 * @brief .
		 * 
		 * @since 
		 */
		ModuleAttributes mAttributes;

	public:

		/**
		 * @brief .
		 * 
		 * @param name A string containing the name of the module.
		 * 
		 * @param category A string containing the category of the module.
		 * 
		 * @param enabled
		 * 
		 * @param callInit
		 * 
		 * 
		 * @since 
		 */
		BModule(std::string const& name, std::string const& category, bool enabled, bool callInitInConstructor);

		/**
		 * @brief .
		 * 
		 * @param name
		 * @param category
		 * @param enabled
		 * 
		 * 
		 * @since 
		 */
		BModule(std::string const& name, std::string const& category, bool enabled);

		/**
		 * @brief .
		 * 
		 * @param name
		 * @param category
		 * 
		 * 
		 * @since 
		 */
		BModule(std::string const& name, std::string const& category);

		/**
		 * @brief .
		 * 
		 * 
		 * 
		 * @since 
		 */
		BModule() = default;

		/**
		 * @brief .
		 * 
		 * 
		 * 
		 * @since 
		 */
		virtual ~BModule();

		virtual void setup() override;

		virtual void init() override;

		virtual void update() override;

		virtual void cleanup() override;

	
		/**
		 * @brief .
		 * 
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		const ModuleAttributes& getAttributes() const;
		
		/**
		 * @brief .
		 * 
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		std::string const& getName() const;

		/**
		 * @brief .
		 * 
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		std::string const& getCategory() const;

		/**
		 * @brief .
		 * 
		 * 
		 * @return 
		 * 
		 * @since 
		 */
		bool isEnabled() const;
	};
#endif
}
