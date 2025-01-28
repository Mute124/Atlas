#pragma once
#include <string>

// Audio module includes
#include "AudioCommon.h"
#include "AudioDeviceConfig.h"
#include "EAudioDeviceType.h"

// what is this used for?
#include "../fs/FilePath.h"

namespace Atlas {	
	/// <summary>
	/// An audio device is a physical <b>OUTPUT</b> device that can be used to play audio. If you are looking for a <b>INPUT</b> device, see <see cref="AudioCaptureDevice" />. If you
	/// are looking for documentation on the forms of <b>OUTPUT</b> devices, see this class's details documentation. 
	/// <para>
	/// \note Please also note that this class's device type defaults to <see cref="EAudioDeviceType::Playback" />. 
	/// </para>
	/// </summary>
	/// <details>
	/// Audio devices come in a few forms:
	///		<list type="bullet">
	/// 		<item>
	/// 			<term>Playback: </term>
	///		 		<description>
	///		 			An audio device that can be used to play audio.
	/// 			</description>
	/// 		</item>
	/// 		<item>
	/// 			<term>Duplex: </term>
	///				<description>
	/// 				An audio device that can be used to both play and record audio.
	/// 			</description>
	/// 		</item>
	///			<item>
	/// 			<term>Loopback: </term>
	/// 			<description>
	/// 				An audio device that can be used to record audio from another device.
	/// 			</description>
	/// 		</item>
	///		</list>
	/// </details>
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
}