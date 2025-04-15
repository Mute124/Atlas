/// @file Common.h
/// 
/// @brief Common includes and definitions that are used throughout the engine. There are going to be very limited amounts of actual engine includes, so this is not a replacement for that. 
/// Instead, this is a way to centralize the includes and definitions that are used throughout the engine.
/// 
/// @todo Add more haptic features
/// 
/// @remarks This file has a few configurations that are based on defines. ATLAS_ENABLE_HAPTICS is used to enable/disable haptic features (ie. controller rumble), whereas ATLAS_ENABLE_EXTENSIONS is used
/// to enable/disable extensions.
#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdint>
#include <string>
#include <thread>
#include <any>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <chrono>
#include <type_traits>

#include "Core.h"

/**
* @brief Since the location of the shared libraries and executables can vary, this is a simple solution to this.
* During Distribution builds, the path is more exact, however the rest are not because these
* will be in the build directory rather than the final distribution directory.
* @since v0.0.2
*/
#define ATLAS_TOP_LAYER_DIR static_cast<std::string>(ATLAS_TOP_LAYER)
#define ATLAS_ASSET_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/game/assets/");
#define ATLAS_DATA_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/game/data/");
#define ATLAS_GAME_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/game/");
#define ATLAS_TEMP_DIR static_cast<std::string>(ATLAS_TOP_LAYER) + static_cast<std::string>("/temp/");

#define ATLAS_NULL_PNG ATLAS_ASSET_DIR + static_cast<std::string>("/engine/null.png")
#define ATLAS_NULL_JPG ATLAS_ASSET_DIR + static_cast<std::string>("/engine/null.jpg")
#define ATLAS_NULL_JPEG static_cast<std::string>(ATLAS_ASSET_DIR) + static_cast<std::string>("/engine/null.jpeg")
#define ATLAS_NULL_GIF ATLAS_ASSET_DIR + static_cast<std::string>("/engine/null.gif")

namespace Atlas {

	using std::string;
	using std::cout;
	using std::endl;
	using std::thread;
	using std::this_thread::sleep_for;
	using std::chrono::milliseconds;
	using std::chrono::seconds;
	using std::unordered_map;

	template<typename T>
	class ReferenceHolder {
	public:

		explicit ReferenceHolder(T& value) : mValue(value) {}

		void set(T& value) {
			mValue = value;
		}

		T& get() {
			return mValue;
		}


	private:
		T& mValue;
	};


#ifdef ATLAS_ENABLE_HAPTICS


#endif

	/// \brief This #ifdef is to enable/disable extensions. Currently this is not implemented, but is planned for the future.
	/// \todo Implement extensions
#ifdef ATLAS_ENABLE_EXTENSIONS

	// Not yet implemented
#ifdef ATLAS_ENABLE_DISCORD_INTEGRATION

#ifndef ATLAS_SILENCE_DISCORD_INTEGRATION_WARNING
#error "Discord integration is not yet implemented. To silence this warning, define ATLAS_SILENCE_DISCORD_INTEGRATION_WARNING."
#endif

#endif

#endif
}
