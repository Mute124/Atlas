#include "AudioManager.h"

void Atlas::AudioManager::init()
{
    /*
    We are going to be initializing multiple engines. In order to save on memory usage we can use a self managed
    resource manager so we can share a single resource manager across multiple engines.
    */
    resourceManagerConfig = ma_resource_manager_config_init();
    resourceManagerConfig.decodedFormat = ma_format_f32;    /* ma_format_f32 should almost always be used as that's what the engine (and most everything else) uses for mixing. */
    resourceManagerConfig.decodedChannels = 0;                /* Setting the channel count to 0 will cause sounds to use their native channel count. */
    resourceManagerConfig.decodedSampleRate = 48000;            /* Using a consistent sample rate is useful for avoiding expensive resampling in the audio thread. This will result in resampling being performed by the loading thread(s). */

    result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize resource manager.");
        initCode = -1;
    }


    /* We're going to want a context so we can enumerate our playback devices. */
    result = ma_context_init(NULL, 0, NULL, &context);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize context.");
        initCode = -1;
    }

    /*
    Now that we have a context we will want to enumerate over each device so we can display them to the user and give
    them a chance to select the output devices they want to use.
    */
    result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, NULL, NULL);
    if (result != MA_SUCCESS) {
        printf("Failed to enumerate playback devices.");
        ma_context_uninit(&context);
        initCode = -1;
    }
}

Atlas::AudioManager::AudioManager()
{
    init();

}
