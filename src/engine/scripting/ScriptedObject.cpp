/**************************************************************************************************
 * @file ScriptedObject.cpp
 * 
 * @brief Implementation of the ScriptedObject header file.
 * 
 * @date August 2025
 * 
 * @since v0.0.1
 ***************************************************************************************************/

#include "ScriptedObject.h"

void Atlas::ScriptedObject::exposeScriptingSymbols() {
	// this function is empty as some modules may not wish to expose any symbols to mods.
}

bool Atlas::ScriptedObject::isScriptingAllowed() const {
	// mScriptingAllowed is a private variable of this class
	return mScriptingAllowed;
}

void Atlas::ScriptedObject::disallowScripting() {
	mScriptingAllowed = false;
}

void Atlas::ScriptedObject::allowScripting() {
	mScriptingAllowed = true;
}

void Atlas::ScriptedObject::toggleScripting() {
	// Just a simple logical switch
	mScriptingAllowed = !mScriptingAllowed;
}

void Atlas::ScriptedObject::setScriptingAllowed(const bool cbAllowed) {
	mScriptingAllowed = cbAllowed;
}
