/**************************************************************************************************
 * @file Platform.h
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#pragma once

#include "Common.h"

namespace Atlas {

	class APlatform {
	public:
		struct Options {
			uint32_t initFlags = 0;
		};

	private:
		
		Options mOptions;

	protected:
		
		void setOptions(Options const& options) { mOptions = options; }

	public:
		explicit APlatform(Options const& options) : mOptions(options) {}

		APlatform::Options const& getOptions() const { return mOptions; }
	};

#ifdef ATLAS_USE_SDL2
	class SDL2Platform : public APlatform {
	public:

		explicit SDL2Platform(Options const& options);

	};
#endif
}
