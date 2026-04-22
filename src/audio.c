#ifndef AUDIO_IMPL
#define AUDIO_IMPL

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "signal.c"

typedef struct {
    ma_device device;
    signal_t *root;
} audio_context_t;

void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frameCount)
{
    (void)input;
    audio_context_t *audio = (audio_context_t*)device->pUserData;
    signal_t *root = audio->root;

    float* out = (float*)output;

    for (ma_uint32 i = 0; i < frameCount; i++) {
        if(!root)
        {
            out[i * 2 + 0] = 0; // left
            out[i * 2 + 1] = 0; // right
        }
        
        float sample = root->func(root);

        out[i * 2 + 0] = sample; // left
        out[i * 2 + 1] = sample; // right
    }
}

void audio_init(audio_context_t *audio, signal_t *root)
{
    audio->root = root;

    ma_device_config config = ma_device_config_init(ma_device_type_playback);

    config.playback.format   = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate        = (unsigned int)SAMPLE_RATE;
    config.dataCallback      = data_callback;
    config.pUserData         = audio;

    ma_device_init(NULL, &config, &audio->device);
    ma_device_start(&audio->device);
}

void audio_terminate(audio_context_t* audio)
{
    ma_device_uninit(&audio->device);
}

#endif