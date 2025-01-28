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
	#define AUDIO_DEVICE_FORMAT    ma_format_f32    // Device output format (miniaudio: float-32bit)
	#define AUDIO_DEVICE_CHANNELS              2    // Device output channels: stereo
	#define AUDIO_DEVICE_SAMPLE_RATE           0    // Device sample rate (device default)

	#define MAX_AUDIO_BUFFER_POOL_CHANNELS    16    // Maximum number of audio pool channels
#endif

#include "../dbg/Errors.h"

namespace Atlas {
#ifdef ATLAS_AUDIO_BACKEND_MINIAUDIO
	using InternalAudioDevice = ma_device;
	using InternalAudioContext = ma_context;
#endif


}