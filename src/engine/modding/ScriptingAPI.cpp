#include "ScriptingAPI.h"
#include "../conf/Config.h"

#ifdef TS_ENABLE_MODDING



/// <summary>
/// Initializes the scripting API.
/// </summary>
/// <returns>an integer value that represents the result of the operation.</returns>
int Atlas::ScriptingAPI::initializeScripting(ScriptingLibraryRegistry const& libraries, ScriptingFunctionRegistry const& functions)
{
	Log("Opening scripting API libraries...");
	this->mLibraries = libraries;
	return 0;
}

void Atlas::ScriptingAPI::registerConfigFunctions() {
	this->mLua.set_function("LookupConfigOption", &GetConfigString);
}

void Atlas::ScriptingAPI::registerFileSystemFunctions()
{
}

#ifdef TS_ENABLE_LUA

void Atlas::ScriptingAPI::openLuaLibraries()
{
	for (auto it = std::begin(Atlas::ScriptingAPI::mLibraries); it != std::end(Atlas::ScriptingAPI::mLibraries); it++) {

		this->mLua.open_libraries(*it);
	}
}

/// <summary>
/// Registers the scripting API for Lua.
/// </summary>
/// <returns>an integer value that represents the result of the operation.</returns>
int Atlas::ScriptingAPI::registerLua()
{
	int result = 0; // TODO: Implement this

	this->openLuaLibraries();
	this->registerConfigFunctions();

	return result;
}

#endif

#ifdef TS_ENABLE_ANGELSCRIPT
/// <summary>
/// Registers the scripting API for AngelScript. This is just for organization sake and it could be merged with the lua version, but it wont be.
/// </summary>
/// <returns>an integer value that represents the result of the operation.</returns>
int Atlas::ScriptingAPI::registerAngelScript()
{
	return 0;
}

#endif

#endif