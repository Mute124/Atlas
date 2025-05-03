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

namespace Atlas {

	/**
	 * @brief An abstract class that provides the virtual functions needed to expose symbols to any
	 * potential mods/scripts. This class is supposed to be statically-typed, meaning it is supposed
	 * to be overriden instead of passing functions to it.
	 * 
	 * @since v0.0.1
	 * 
	 * @todo Add the ability for this class to add events aswell.
	 */
	class ScriptedObject {
	public:

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
		virtual void exposeScriptingSymbols() {
			// this function is empty as some modules may not wish to expose any symbols to mods.
		}
	};
}
