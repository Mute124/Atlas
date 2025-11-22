/**************************************************************************************************
 * @file Platform.cpp
 * 
 * @brief .
 * 
 * @date November 2025
 * 
 * @since v
 ***************************************************************************************************/
#include "Platform.h"

#ifdef ATLAS_USE_SDL2
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_stdinc.h>
	#include <SDL2/SDL_video.h>
	#include <SDL2/SDL_events.h>
#endif


#ifdef ATLAS_USE_SDL2

Atlas::SDL2Platform::SDL2Platform(Options const& options) : APlatform(options) {
	


	if (SDL_Init(options.initFlags) != 0) {

	}
}


#endif