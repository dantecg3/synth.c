#ifndef SIGNAL_IMPL
#define SIGNAL_IMPL

#include <math.h>

#include "arena.c"

#define SAMPLE_RATE 48000.0f
#define TWO_PI 6.28318530718f

typedef struct signal_t signal_t;
typedef float (*signal_func)(signal_t* self);

struct signal_t {
    signal_func func;
    void* data;
};

typedef struct {
    float phase;
    float freq;
    float amp;
} signal_sine_data_t;

typedef struct {
    signal_t sig;
    signal_sine_data_t *data;
} signal_sine_handle_t;

typedef struct {
    signal_t a;
    signal_t b;
} signal_add_data_t;

float signal_sine_eval(signal_t *self)
{
    signal_sine_data_t* d = (signal_sine_data_t*)self->data;
    
    float out = d->amp * sinf(d->phase);
    
    d->phase += TWO_PI * d->freq / SAMPLE_RATE;
    if (d->phase > TWO_PI)
        d->phase -= TWO_PI;
    
    return out;
}

signal_sine_handle_t signal_sine(arena_t *arena, float freq, float amp)
{
    signal_sine_data_t* d = arena_alloc(arena, sizeof(signal_sine_data_t));

    d->phase = 0;
    d->freq = freq;
    d->amp = amp;

    return (signal_sine_handle_t){
        .sig = (signal_t){
            .func = signal_sine_eval,
            .data = d
        },
        .data = d
    };
}

float signal_add_eval(signal_t *self)
{
    signal_add_data_t* d = (signal_add_data_t*)self->data;
    
    float a = d->a.func(&d->a);
    float b = d->b.func(&d->b);

    return a + b;
}

signal_t signal_add(arena_t *arena, signal_t a, signal_t b)
{
    signal_add_data_t* add = arena_alloc(arena, sizeof(signal_add_data_t));

    add->a = a;
    add->b = b;

    return (signal_t) {
        .func = signal_add_eval,
        .data = add
    };
}

#endif