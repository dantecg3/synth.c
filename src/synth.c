#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "bitmap.c"
#include "arena.c"
#include "input.c"
#include "signal.c"
#include "audio.c"

double now_seconds()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

int main()
{
    bool running = true;
    input_context_t input = {0};
    audio_context_t audio = {0};
    arena_t arena = {};

    signal_sine_handle_t osc1 = signal_sine(&arena, 261.63f, 0.3f);
    
    input_init(&input);
    audio_init(&audio, &osc1.sig);
    
    while (running)
    {
        input_update(&input);
        
        if (input_is_key_pressed(&input, 'q'))
            osc1.data->freq = 261.63f; // C4

        if (input_is_key_pressed(&input, 'w'))
            osc1.data->freq = 293.66f; // D4

        if (input_is_key_pressed(&input, 'e'))
            osc1.data->freq = 329.63f; // E4

        if (input_is_key_pressed(&input, 'r'))
            osc1.data->freq = 349.23f; // F4

        if (input_is_key_pressed(&input, 't'))
            osc1.data->freq = 392.00f; // G4

        if (input_is_key_pressed(&input, 'y'))
            osc1.data->freq = 440.00f; // A4

        if (input_is_key_pressed(&input, 'u'))
            osc1.data->freq = 493.88f; // B4

        if (input_is_key_pressed(&input, 'i'))
            osc1.data->freq = 261.63f * 2.0f; // C5

        if (input_is_key_pressed(&input, 'l'))
            break;
        
        usleep(100);
    }

    input_terminate(&input);
    audio_terminate(&audio);

    return 0;
}