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

//Atlas::SDL2Platform::SDL2InitInfo::SDL2InitInfo(bool bInitEverything) 
//	: bInitEverything(bInitEverything) {}

Atlas::SDL2Platform::SDL2Platform(Options const& options) : APlatform(options) {
	/*
	* Step 1: Initialize Logging
	* Step 2: Initialize file system
	* Step 3: Initialize SDL
	*/

	uint32_t sdlInitFlags = 0;

	const bool cbInitEverything = options.enabledModules.contains(Atlas::EModuleType::Everything);

	bool bInitVideo;
	bool bInitAudio;
	bool bInitInput;
	bool bInitGameControllers;
	bool bInitJoysticks;
	bool bInitNetworking;
	bool bInitHaptics;
	bool bInitEvents;
	bool bInitPhysics;
	bool bInitDebug;
	bool bInitScripting;
	bool bInitModding;

	if (cbInitEverything) {
		sdlInitFlags |= SDL_INIT_EVERYTHING;
	}

	else {
		// This is here to reduce verbosity.
		using enum Atlas::EModuleType;

		if (options.enabledModules.contains(Video)) {
			bInitVideo = true;
			
			sdlInitFlags |= SDL_INIT_VIDEO;
		}

		if (options.enabledModules.contains(Audio)) {
			bInitAudio = true;
			
			sdlInitFlags |= SDL_INIT_AUDIO;
			
		}

		if (options.enabledModules.contains(Input)) {
			bInitInput = true;
			bInitGameControllers = true;
			bInitJoysticks = true;

			sdlInitFlags |= SDL_INIT_JOYSTICK;
			sdlInitFlags |= SDL_INIT_GAMECONTROLLER;

			if (options.enabledModules.contains(Haptics)) {
				bInitHaptics = true;
				sdlInitFlags |= SDL_INIT_HAPTIC;
			}
		}

		if (options.enabledModules.contains(Networking)) {
			bInitNetworking = true;
		}

		if (options.enabledModules.contains(Events)) {
			bInitEvents = true;
			
			sdlInitFlags |= SDL_INIT_EVENTS;
		}

		if (options.enabledModules.contains(Physics)) {
			bInitPhysics = true;
		}

		if (options.enabledModules.contains(Debug)) {
			bInitDebug = true;
		}

		if (options.enabledModules.contains(Scripting)) {
			bInitScripting = true;
		}

		if (options.enabledModules.contains(Modding)) {
			bInitModding = true;
		}
	}

	if (SDL_Init(sdlInitFlags) != 0) {

	}
}

void Atlas::SDL2Platform::initSDL2(uint32_t sdlInitFlags)
{
}


#endif


