#pragma once

namespace Atlas {
	class EngineModule {
	private:
		bool mIsReady = false;
		bool mIsEnabled = false;
	
	public:
		virtual void exposeSymbols() {
			// this function is empty as some modules may not wish to expose any symbols to mods.
		}

		virtual bool isReady() { return this->mIsReady; }
		
		virtual bool isEnabled() { return this->mIsEnabled; }

	};
}