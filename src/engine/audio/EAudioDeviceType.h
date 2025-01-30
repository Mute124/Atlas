#pragma once
#include <cstdint>

namespace Atlas {	
	/// <summary>
	/// This enum represents the different types of audio devices. It is also used to match the values of the ma_device_type enum, which is why PLAYBACK_DEVICE_TYPE is set to 1. 
	/// This enum also inherits from int8_t because there are only 4 possible values.
	/// 
	/// \warning If you are trying to set an audio device as a recording, please note that this is NOT the proper enum to use because this only applies to OUTPUT devices.
	/// <para>
	/// The values are as follows:
	/// <para/>
	///		<list type="bullet">
	///			<item>
	///				<term>
	///					<b>UNKNOWN_DEVICE_TYPE: </b>	
	///				</term>
	/// 			<description>
	/// 				With a value of -1, this represents an unknown device type. Typically, this is used when a device is not found, it is not initialized, or it is not available 
	///					for some other reason. 
	///					<para/>
	/// 			</description>
	///			</item>
	/// 		<item>
	///				<term>
	///					<b>PLAYBACK_DEVICE_TYPE: </b>
	///				</term>
	/// 			<description>
	/// 				With a value of 1, this represents a playback device type. 
	///					<para/>
	/// 			</description>
	/// 		</item>
	/// 		<item>
	/// 			<term>
	/// 				<b>DUPLEX_DEVICE_TYPE: </b>
	/// 			</term>
	/// 			<description>
	/// 				With a value of 3, this represents a duplex device type.
	/// 				<para/>
	/// 			</description>
	/// 		</item>
	/// 		<item>
	/// 			<term>
	/// 				<b>LOOPBACK_DEVICE_TYPE: </b>
	/// 			</term>
	/// 			<description>
	/// 				With a value of 4, this represents a loopback device type.
	/// 				<para/>
	/// 			</description>
	/// 		</item>
	///		</list>
	/// </para>
	/// </summary>
	enum class EAudioDeviceType : int8_t {

		/**
		 * @brief An unknown device type.
		 * @remarks This is typically used when a device is not found, it is not initialized, or it is not available for some other reason.
		 */
		UNKNOWN_DEVICE_TYPE = -1,
		PLAYBACK_DEVICE_TYPE = 1,
		DUPLEX_DEVICE_TYPE = 3,
		LOOPBACK_DEVICE_TYPE = 4,
	};
}