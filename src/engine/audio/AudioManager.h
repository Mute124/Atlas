#pragma once
#include "AudioCommon.h"

namespace Atlas {
	class AudioManager {
	private:
		ma_context mAudioContext;
		ma_engine mAudioEngine;
		ma_context_config mAudioContextConfig;

        
        void init();
	public:
        int initCode;

		AudioManager();
	};
}