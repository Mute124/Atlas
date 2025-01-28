#include "AudioDevice.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.
}

Atlas::AudioDevice::AudioDevice(FilePath const& deviceSettingsPath, AudioDeviceConfig* config)
{
}

Atlas::AudioDevice::AudioDevice(FilePath const& deviceSettingsPath) {}

Atlas::AudioDevice::AudioDevice() {
	this->mDeviceConfig = ma_device_config_init(ma_device_type_playback);
    this->mDeviceConfig.playback.pDeviceID = NULL;  // NULL for the default playback AUDIO.System.device
    this->mDeviceConfig.playback.format = AUDIO_DEVICE_FORMAT;
    this->mDeviceConfig.playback.channels = AUDIO_DEVICE_CHANNELS;
    this->mDeviceConfig.capture.pDeviceID = NULL;  // NULL for the default capture AUDIO.System.device
    this->mDeviceConfig.capture.format = ma_format_s16;
    this->mDeviceConfig.capture.channels = 1;
    this->mDeviceConfig.sampleRate = AUDIO_DEVICE_SAMPLE_RATE;
    this->mDeviceConfig.dataCallback = data_callback;
    this->mDeviceConfig.pUserData = NULL;
}
