#ifndef ARENA_IMPL
#define ARENA_IMPL

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ARENA_SIZE 1024 * 1024
typedef struct {
    uint8_t buffer[ARENA_SIZE];
    size_t offset;
} arena_t;

void* arena_alloc(arena_t* a, size_t size)
{
    size = (size + 7) & ~7;

    if (a->offset + size > ARENA_SIZE)
        return NULL;

    void* ptr = a->buffer + a->offset;
    a->offset += size;

    return ptr;
}

#endif