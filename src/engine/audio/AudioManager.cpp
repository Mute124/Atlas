#include "AudioManager.h"
#include <iostream>
#include <functional>
void Atlas::AudioManager::init()
{
	mAudioContextConfig = ma_context_config_init();
	ma_log_callback_init(nullptr, [](void* pUserData, ma_uint32 level, const char* pMessage) {
			std::cout << "[Audio] " << pMessage << std::endl;
	});

	ma_result result = ma_context_init(NULL, 0, &mAudioContextConfig, &mAudioContext);
	if(result != MA_SUCCESS) {
		std::cout << "Failed to initialize audio context" << std::endl;
	}


}

Atlas::AudioManager::AudioManager()
{
	init();

}
