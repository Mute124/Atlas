#pragma once

#include "../scripting/ScriptedObject.h"

namespace Atlas {
	class EngineModule : public ScriptedObject {
	private:
		bool mIsReady = false;
		bool mIsEnabled = false;
	
	public:


		virtual bool isReady() { return this->mIsReady; }
		
		virtual bool isEnabled() { return this->mIsEnabled; }

	};
}