#pragma once
#ifdef ATLAS_ENABLE_MODDING
	//#include <angelscript.h>
	#ifdef ATLAS_ENABLE_ANGELSCRIPT
		
	#endif

#endif

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <assert.h>
#include <string.h>  // strstr()
#include <conio.h>   // kbhit(), getch()
#include <windows.h> // timeGetTime()
#include <angelscript.h>
#include "../../EReturnCode.h"
#include "../add_on/scriptstdstring/scriptstdstring.h"

namespace Atlas {	
	class AngelScriptAPI {
	private:
		asIScriptEngine* mScriptEngine = nullptr;
		asIScriptContext* mScriptEngineContext = nullptr;

		static inline asIScriptEngine* CreateScriptEngine() {
			return asCreateScriptEngine(ANGELSCRIPT_VERSION);
		}

	protected:
		AngelScriptAPI() : mScriptEngine(CreateScriptEngine()) {
			if(mScriptEngine == nullptr) {
				throw std::runtime_error("Failed to create AngelScript engine");
			}

		}
	};
}