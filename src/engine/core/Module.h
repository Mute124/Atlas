#pragma once

namespace Atlas {
	class EngineModule {
	public:
		virtual void exposeSymbols() {
			// this function is empty as some modules may not wish to expose any symbols to mods.
		}
	};
}