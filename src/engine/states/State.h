#pragma once
#include <string>
#ifdef ATLAS_ENABLE_MODDING

#endif

namespace Atlas {
#ifdef ATLAS_ENABLE_MODDING
	class GameState {
#else
	class GameState {
#endif
	private:
		const std::string mName;
	public:
		const std::string& getName() const { return mName; }
	};

}