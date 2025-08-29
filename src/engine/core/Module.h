#pragma once

#include "../scripting/ScriptedObject.h"

#include <string>

namespace Atlas {
	class EngineModule : public ScriptedObject {
	private:

		bool mIsReady = false;

		bool mIsEnabled = false;
	
		const std::string mThreadHandlerID;

	public:

		virtual bool isReady() { return this->mIsReady; }
		
		virtual bool isEnabled() { return this->mIsEnabled; }

	};
}