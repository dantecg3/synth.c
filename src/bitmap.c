#ifndef BITMAP_IMPL
#define BITMAP_IMPL

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define BITMAP_MAX_SIZE 64
typedef struct bitmap_t {
    uint8_t data[BITMAP_MAX_SIZE];
    size_t  size;
} bitmap_t;

void bitmap_set_bit(bitmap_t* bitmap, size_t bit)
{
    size_t byte = bit >> 3;
    if (byte >= bitmap->size) return;
    bitmap->data[byte] |= (1 << (bit & 7));
}

void bitmap_unset_bit(bitmap_t* bitmap, size_t bit)
{
    size_t byte = bit >> 3;
    if (byte >= bitmap->size) return;
    bitmap->data[byte] &= ~(1 << (bit & 7));
}

bool bitmap_get_bit(const bitmap_t* bitmap, size_t bit)
{
    size_t byte = bit >> 3;
    if (byte >= bitmap->size) return false;
    return bitmap->data[byte] & (1 << (bit & 7));
}

void bitmap_set_all(bitmap_t* bitmap)
{
    for (size_t i = 0; i < bitmap->size; ++i)
        bitmap->data[i] = 0xFF;
}

void bitmap_unset_all(bitmap_t* bitmap)
{
    for (size_t i = 0; i < bitmap->size; ++i)
        bitmap->data[i] = 0x00;
}

void bitmap_copy(bitmap_t *dst, const bitmap_t *src)
{
    size_t max = (dst->size < src->size) ? dst->size : src->size;

    for (size_t i = 0; i < max; i++)
        dst->data[i] = src->data[i];
}

void bitmap_init(bitmap_t *bitmap, size_t size)
{
    if (size > BITMAP_MAX_SIZE)
        size = BITMAP_MAX_SIZE;

    bitmap->size = size;
    bitmap_unset_all(bitmap);
}

#endif