#pragma once
#include "AudioCommon.h"

namespace Atlas {
	class AudioManager {
	private:
		ma_context mAudioContext;
		ma_engine mAudioEngine;

        ma_result result;
        ma_context context;
        ma_resource_manager_config resourceManagerConfig;
        ma_resource_manager resourceManager;
        ma_engine engines[MAX_DEVICES];
        ma_device devices[MAX_DEVICES];
        ma_uint32 engineCount = 0;
        ma_uint32 iEngine;
        ma_device_info* pPlaybackDeviceInfos;
        ma_uint32 playbackDeviceCount;
        ma_uint32 iAvailableDevice;
        ma_uint32 iChosenDevice;
        ma_sound sounds[MAX_SOUNDS];
        ma_uint32 soundCount;
        ma_uint32 iSound;
        
        void init();
	public:
        int initCode;

		AudioManager();
	};
}