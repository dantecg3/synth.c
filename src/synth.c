#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "bitmap.c"
#include "arena.c"
#include "string.c"
#include "input.c"
#include "signal.c"
#include "audio.c"
#include "render.c"

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
    render_screen_t screen = {0};
    arena_t arena = {};

    signal_sine_handle_t osc1 = signal_sine_ctr(&arena, 261.63f, 0.2f);
    string_t signal_str = string_ctr("Sine: ");
    
    input_init(&input);
    audio_init(&audio, &osc1.sig);
    
    render_clear(&screen, ' ');
    while (running) {
        input_update(&input);
        input_mouse_event_t e = input_handle_mouse_event(&input);
        
        render_clear_terminal();
        
        if(e.type == MOUSE_PRESS)
            render_cell(&screen, e.x, e.y, 'X');

        string_format(&signal_str, "Sine: freq:%.2f amp:%.2f", osc1.data->freq, osc1.data->amp);
        render_string(&screen, 0, RENDER_SCREEN_HEIGHT-1, string_view(signal_str.data));
        
        render_present(&screen);

        if (input_is_key_pressed(&input, 'l'))
            break;

        fflush(stdout);
        usleep(16000);
    }

    input_terminate(&input);
    audio_terminate(&audio);

    return 0;
}