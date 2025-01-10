/**
* \file ScriptingAPI.h
* 
* \brief This is what will handle all of Atlas's scripting. Currently only Lua is supported, but AngelScript is planned for a future date. Along with this, the scripting API is responsible for
* the console, loading/unloading lua libraries, etc.
* \includegraph
* 
*/
#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <string>
#include "../dbg/Logging.h"

#ifdef TS_ENABLE_MODDING
	#ifdef TS_ENABLE_LUA
		/// \brief Protects against safety issues when using lua because it makes the API more stable.
		#define SOL_ALL_SAFETIES_ON 1

		#include <sol/state.hpp>
		#include <sol/state_view.hpp>
		#include <sol/types.hpp>
		#include <sol/forward.hpp>
	#endif

	#ifdef TS_ENABLE_ANGELSCRIPT

	#endif
#endif

#include "../utils/Singleton.h"

namespace Atlas {
	

#ifdef TS_ENABLE_MODDING
	// Logic to determine what using statements to use. This is mainly to avoid code duplication.
	#if defined(TS_ENABLE_LUA) && defined(TS_ENABLE_ANGELSCRIPT)
	
	#elif defined(TS_ENABLE_LUA) || defined(TS_ENABLE_ANGELSCRIPT) // only one is defined since it would have triggered the previous if otherwise.
		#ifdef TS_ENABLE_LUA

		#endif // TS_ENABLE_LUA

		#ifdef TS_ENABLE_ANGELSCRIPT

		#endif // TS_ENABLE_ANGELSCRIPT
	#else // Preprocessor else of defined(TS_ENABLE_LUA) && defined(TS_ENABLE_ANGELSCRIPT)



	#endif // defined(TS_ENABLE_LUA) && defined(TS_ENABLE_ANGELSCRIPT)



	/// <summary>
	/// Represents a function that can be exported to lua. The reason for it's existence is because it makes it easier to export functions to lua.
	/// </summary>
	struct ExportedFunction {
		const std::string cName; // What the function is called (i.e "Print")
#ifdef TS_ENABLE_LUA
		sol::function cFunction;
#endif
	};

	using ScriptingLibraryRegistry = std::vector<sol::lib>;
	using ScriptingFunctionRegistry = std::unordered_map<std::string, ExportedFunction>;

	/// <summary>
	/// Everything related to the scripting API, i.e loading/unloading libraries, registering functions, etc. As is mentioned at the file's documentation, only lua is supported at the moment.
	/// </summary>
	/// <seealso cref="Singleton&lt;ScriptingAPI&gt;" />
	class ScriptingAPI : public Singleton<ScriptingAPI> {
	public:
		ScriptingAPI() = default;

		/// <summary>
		/// Initializes the scripting API.
		/// </summary>
		/// <returns>an integer value that represents the result of the operation.</returns>
		int initializeScripting(ScriptingLibraryRegistry const& libraries, ScriptingFunctionRegistry const& functions);

		template<typename T, typename ...Args>
		int registerFunction(const std::string& name, T function, Args... args) {
		
			mLua.new_usertype<T>(args);
		}
		


#ifdef TS_ENABLE_LUA
		/// <summary>
		/// Registers the scripting API for Lua.
		/// </summary>
		/// <returns>an integer value that represents the result of the operation.</returns>
		int registerLua();

		/// <summary>
		/// Returns mLua as a state_view.
		/// </summary>
		/// <returns>a state_view that represents the lua state (mLua).</returns>
		sol::state_view getState() { return mLua.operator lua_State * (); }
#endif

#ifdef TS_ENABLE_ANGELSCRIPT
		/// <summary>
		/// Registers the scripting API for AngelScript. This is just for organization sake and it could be merged with the lua version, but it wont be.
		/// </summary>
		/// <returns>an integer value that represents the result of the operation.</returns>
		int registerAngelScript();
				
#endif



	private:


#ifdef TS_ENABLE_LUA
		sol::state mLua;
		ScriptingLibraryRegistry mLibraries;
		ScriptingFunctionRegistry mFunctions;
		
		/// <summary>
		/// Exposes the lua libraries in mLibraries to mLua. See \ref ScriptingLibraryRegistry for more information.
		/// </summary>
		void openLuaLibraries();
#endif

		/// <summary>
		/// Exposes all functions that fall under the perview of config. As of version 0.0.3, this is only the lookup function and more will be added in the future. 
		/// </summary>
		void registerConfigFunctions();

		/// <summary>
		/// Exposes all functions that fall under the perview of file system. This includes file loading, reading, writing, etc. 
		/// </summary>
		void registerFileSystemFunctions();
	};

#endif
}
