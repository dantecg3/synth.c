#ifndef INPUT_IMPL
#define INPUT_IMPL

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "bitmap.c"

#define INPUT_BITCOUNT 256

typedef enum { 
    MOUSE_NONE, MOUSE_PRESS, 
    MOUSE_RELEASE, MOUSE_DRAGGING 
} input_mouse_event_type_t;

typedef struct {
    uint32_t x, y;
    input_mouse_event_type_t type;
} input_mouse_event_t;

typedef struct {
    struct termios old_termios, new_termios;
    
    bitmap_t key_state;
    bool mouse_holding;
    uint32_t mouse_x, mouse_y;
    input_mouse_event_t mouse_event;
} input_context_t;

void input_init(input_context_t *ctx)
{
    struct termios *new = &ctx->new_termios;
    struct termios *old = &ctx->old_termios;

    tcgetattr(STDIN_FILENO, old);
    *new = *old;

    new->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, new);

    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    bitmap_init(&ctx->key_state, INPUT_BITCOUNT);

    ctx->mouse_event.type = MOUSE_NONE;
    ctx->mouse_holding = false;

    // mouse modes
    printf("\x1b[?1000h");
    printf("\x1b[?1003h");
    printf("\x1b[?1006h");
    fflush(stdout);
}

void input_terminate(input_context_t *ctx)
{
    printf("\x1b[?1000l");
    printf("\x1b[?1003l");
    printf("\x1b[?1006l");
    fflush(stdout);

    tcsetattr(STDIN_FILENO, TCSANOW, &ctx->old_termios);
}

void input_update(input_context_t *ctx)
{
    bitmap_unset_all(&ctx->key_state);
    ctx->mouse_event.type = MOUSE_NONE;

    char buf[512];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));

    if (n <= 0)
        return;

    uint32_t old_x = ctx->mouse_x;
    uint32_t old_y = ctx->mouse_y;

    for (ssize_t i = 0; i < n; i++) {
        if (buf[i] == 27 && i + 2 < n && buf[i+1] == '[' && buf[i+2] == '<') {
            int b, x, y;
            char type;

            if (sscanf(&buf[i], "\x1b[<%d;%d;%d%c", &b, &x, &y, &type) == 4) {
                ctx->mouse_x = x - 1;
                ctx->mouse_y = y - 1;

                ctx->mouse_event.x = ctx->mouse_x;
                ctx->mouse_event.y = ctx->mouse_y;

                if (type == 'M') {
                    ctx->mouse_holding = true;
                    ctx->mouse_event.type = MOUSE_PRESS;
                } else if (type == 'm') {
                    ctx->mouse_holding = false;
                    ctx->mouse_event.type = MOUSE_RELEASE;
                }
                continue;
            }
        }

        bitmap_set_bit(&ctx->key_state, (unsigned char)buf[i]);
    }

    //FIX DRAGGING
    if (ctx->mouse_holding && (ctx->mouse_x != old_x || ctx->mouse_y != old_y)) {
        ctx->mouse_event.type = MOUSE_DRAGGING;
        ctx->mouse_event.x = ctx->mouse_x;
        ctx->mouse_event.y = ctx->mouse_y;
    }
}

bool input_is_key_pressed(const input_context_t *ctx, size_t key) 
{
    return bitmap_get_bit(&ctx->key_state, key);
}

input_mouse_event_t input_handle_mouse_event(const input_context_t *ctx)
{
    return ctx->mouse_event;
}

#endif