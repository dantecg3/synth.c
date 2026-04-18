#ifndef INPUT_IMPL
#define INPUT_IMPL

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "bitmap.c"

#define INPUT_BITMAP_SIZE 32
typedef struct input_context_t 
{
    struct termios old_termios, new_termios;
    bitmap_t key_state;
} input_context_t;

void input_init(input_context_t *ctx) 
{
    struct termios* new = &ctx->new_termios;
    struct termios* old = &ctx->old_termios;

    tcgetattr(STDIN_FILENO, old);
    *new = *old;
    new->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, new);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    fflush(stdout);

    bitmap_init(&ctx->key_state, INPUT_BITMAP_SIZE);
}

void input_terminate(input_context_t *ctx) 
{
    tcsetattr(STDIN_FILENO, TCSANOW, &ctx->old_termios);
}

void input_update(input_context_t *ctx) 
{
    bitmap_t *keystate = &ctx->key_state;
    bitmap_unset_all(keystate);

    char c;
    while (read(STDIN_FILENO, &c, 1) > 0)
        bitmap_set_bit(keystate, (size_t)c);
}

static inline bool input_is_key_pressed(const input_context_t *ctx, size_t key) 
{
    return bitmap_get_bit(&ctx->key_state, key);
}

#endif