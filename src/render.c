#ifndef RENDER_INCL
#define RENDER_INCL

#include <stdio.h>
#include <stdint.h>

#include "string.c"

#define RENDER_SCREEN_WIDTH 100
#define RENDER_SCREEN_HEIGHT 30

typedef struct {
    char cells[RENDER_SCREEN_WIDTH * RENDER_SCREEN_HEIGHT];
} render_screen_t;

void render_cell(render_screen_t *screen, uint32_t x, uint32_t y, char c)
{
    if(!screen)
        return;
    if (x >= RENDER_SCREEN_WIDTH || y >= RENDER_SCREEN_HEIGHT)
        return;

    uint32_t index = y * RENDER_SCREEN_WIDTH + x;
    screen->cells[index] = c;
}

void render_string(render_screen_t *screen, uint32_t x, uint32_t y, string_view_t string)
{
    if(!screen || !string.data)
        return;
    if ((x + string.length) >= RENDER_SCREEN_WIDTH || y >= RENDER_SCREEN_HEIGHT)
        return;

    for(size_t i = 0; i < string.length; ++i)
        render_cell(screen, x + i, y, string.data[i]);
}

void render_fill(render_screen_t *screen, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, char c)
{
    for (uint32_t x = x1; x <= x2; ++x) {
        for (uint32_t y = y1; y <= y2; ++y) {
            uint32_t i = y * RENDER_SCREEN_WIDTH + x;
            screen->cells[i] = c;
        }
    }
}

void render_clear(render_screen_t *screen, char c) 
{
    render_fill(screen, 0, 0, RENDER_SCREEN_WIDTH-1, RENDER_SCREEN_HEIGHT-1, c);
}

void render_clear_terminal()
{
    printf("\x1b[2J");
    printf("\x1b[?25l");
    fflush(stdout);
}

void render_present(render_screen_t *s)
{
    printf("\x1b[H");

    for (int y = 0; y < RENDER_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < RENDER_SCREEN_WIDTH; x++) {
            putchar(s->cells[y * RENDER_SCREEN_WIDTH + x]);
        }
        putchar('\n');
    }

    fflush(stdout);
}

#endif