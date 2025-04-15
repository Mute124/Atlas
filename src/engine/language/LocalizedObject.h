
#pragma once

#include <string>
#include "Language.h"

namespace Atlas {
	/**
	 * @brief A localized object is one that is localized to a specific language.
	 * This means that it can dynamically change depending on the language without
	 * much effort. All it needs is a language and a path to the file. Should the
	 * language change, this class contains a function to update it. This class
	 * is also the base class for all objects that need to be localized. All
	 * functions within this are @b pure virtuals. 
	 * 
	 * @since v0.0.9
	 */
	class LocalizedObject {
	private:
		Language* mLanguage;
	public:
		
		/**
		 * @brief 
		 */
		virtual void localize() = 0;
		
		virtual void updateLanguage(Language* language) { 
			mLanguage = language; 
		}
	};
}