#pragma once
#include <string>

// Audio module includes
#include "AudioCommon.h"
#include "AudioDeviceConfig.h"
#include "EAudioDeviceType.h"

// what is this used for?
#include "../fs/FilePath.h"

namespace Atlas {	


#ifndef ATLAS_RAYLIB_AUDIO

	/**
	 * @brief An audio device is a physical @b OUTPUT device that can be used to play audio.
	 * If you are looking for a @b INPUT device, see AudioCaptureDevice. If you are looking for documentation
	 * on the forms of @b OUTPUT devices, see this class's details documentation. 

	 * @remarks An audio device comes in a few forms: 
	 * 		<list type="bullet">
	 * 			<item>
	 * 				<term>@ref EAudioDeviceType::PLAYBACK_DEVICE_TYPE: </term>
	 *		 			<description>
	 *			 			is the default and is an audio device that can be used to play audio.
	 *	 			</description>
	 *	 		</item>
	 *	 		<item>
	 *	 			<term>@ref EAudioDeviceType::DUPLEX_DEVICE_TYPE: </term>
	 *					<description>
	 *	 				is an audio device that can be used to both play and record audio.
	 *	 			</description>
	 *	 		</item>
	 *			<item>
	 *	 			<term>@ref EAudioDeviceType::LOOPBACK_DEVICE_TYPE: </term>
	 *	 			<description>
	 *	 				is an audio device that can be used to record audio from another device.
	 *	 			</description>
	 *	 		</item>
	 *		</list>
	 * @note Please also note that this class's device type defaults to @ref EAudioDeviceType::PLAYBACK_DEVICE_TYPE
	 * @since v0.0.9
	 */
	class AudioDevice {
	private:
		InternalAudioDevice mDevice;
		std::string mAlias;
		EAudioDeviceType mDeviceType;
		unsigned int mDeviceID;
		friend class AudioManager;
		ma_device_config mDeviceConfig;

		
	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="AudioDevice"/> class.
		/// </summary>
		/// <param name="deviceSettingsPath">The device settings path.</param>
		/// <param name="config">The configuration.</param>
		explicit AudioDevice(FilePath const& deviceSettingsPath, AudioDeviceConfig* config);
		
		/// <summary>
		/// Initializes a new instance of the <see cref="AudioDevice"/> class.
		/// </summary>
		/// <param name="deviceSettingsPath">The device settings path.</param>
		explicit AudioDevice(FilePath const& deviceSettingsPath);
		
		/// <summary>
		/// Initializes a new instance of the <see cref="AudioDevice"/> class.
		/// </summary>
		AudioDevice();
		
	};

#endif
}