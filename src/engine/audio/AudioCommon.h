/**
* @file AudioCommon.h
* @tableofcontents{html}
* @brief Common audio types
* @details This file contains common audio types like @ref Atlas::InternalAudioDevice and @ref Atlas::InternalAudioContext. The functionality depends on the audio backend. For example,
* if the audio backend is @ref ATLAS_AUDIO_BACKEND_MINIAUDIO, then the @ref InternalAudioDevice and @ref InternalAudioContext are Miniaudio types. 
*/
#pragma once
#define ATLAS_AUDIO_BACKEND_MINIAUDIO

#ifdef ATLAS_AUDIO_BACKEND_MINIAUDIO
	#include <miniaudio.h>

	#define MAX_DEVICES 2
	#define MAX_SOUNDS  32

#endif

#include "../dbg/Errors.h"

namespace Atlas {
#ifdef ATLAS_AUDIO_BACKEND_MINIAUDIO
	using InternalAudioDevice = ma_device;
	using InternalAudioContext = ma_context;
#endif


}