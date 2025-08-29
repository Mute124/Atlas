/**************************************************************************************************
 * @file ScriptedObject.h
 * 
 * @brief .
 * 
 * @date May 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/
#pragma once

//#include "../core/AtlasEngine.h"

namespace Atlas {

	class ISymbol {

	};

	class ScriptingSymbolExposer {
	public:

		virtual void exposeSymbol(ISymbol* cSymbolToExpose) = 0;

		virtual void exposeSymbols() = 0;

	};

	/**
	 * @brief An abstract class that provides the virtual functions needed to expose symbols to any
	 * potential mods/scripts. This class is supposed to be statically-typed, meaning it is supposed
	 * to be overriden instead of passing functions to it.
	 * 
	 * @details This class is supposed to be inherited by any object that is supposed to be scripted.
	 * Among the features that this class should be able to provide are:
	 * 
	 * 1. Exposing symbols for mods to use.
	 * 
	 * @since v0.0.1
	 * 
	 * @todo Add the ability for this class to add events aswell.
	 */
	class ScriptedObject {
	private:
	
		bool mScriptingAllowed = true;

	public:

		//ScriptingSymbolExposer* mSymbolExposer = nullptr;

		/**
		 * @brief A virtual function that is called when scripting is initializing. Override this if
		 * the child class needs to contribute symbols to the overall scripting API. This can include
		 * functions, globals, etc.
		 * 
		 * @note Leave empty if you do not want anything to be exposed.
		 * 
		 * @since v0.0.1
		 * 
		 * @todo Add an interface parameter that provides a common interface for this function to
		 * actually expose symbols with. As of now, it basically does not define a way that the
		 * module is supposed to use when doing this.
		 * 
		 * @sa @ref EngineModule 
		 */
		virtual void exposeScriptingSymbols();

		/**
		 * @brief Returns whether or not this object is allowed to be scripted. 
		 * 		 * 
		 * @note If you are not doing anything fancy when you are inheriting
		 * this class, you do not need to override this function.
		 * 
		 * @returns A boolean that indicates whether or not this object is allowed to be scripted. Assuming
		 * standard implementation, this will return the value of mScriptingAllowed.
		 * 
		 * @since v0.0.1
		 * 
		 * @sa @ref mScriptingAllowed
		 */
		bool isScriptingAllowed() const;

		void disallowScripting();

		void allowScripting();

		void toggleScripting();

		void setScriptingAllowed(const bool cbAllowed);
	};
}
