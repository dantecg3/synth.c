#ifndef BITMAP_IMPL
#define BITMAP_IMPL

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define BITMAP_MAX_BYTES 128

typedef struct {
    uint8_t data[BITMAP_MAX_BYTES];
    size_t bitcount;
} bitmap_t;

size_t bitmap_byte_count(const bitmap_t* b)
{
    return (b->bitcount + 7) >> 3;
}

void bitmap_set_bit(bitmap_t* b, size_t bit)
{
    if (bit >= b->bitcount) return;
    b->data[bit >> 3] |= (uint8_t)(1u << (bit & 7));
}

void bitmap_unset_bit(bitmap_t* b, size_t bit)
{
    if (bit >= b->bitcount) return;
    b->data[bit >> 3] &= (uint8_t)~(1u << (bit & 7));
}

bool bitmap_get_bit(const bitmap_t* b, size_t bit)
{
    if (bit >= b->bitcount) return false;
    return (b->data[bit >> 3] >> (bit & 7)) & 1u;
}

void bitmap_set_all(bitmap_t* b)
{
    size_t bytes = bitmap_byte_count(b);

    for (size_t i = 0; i < bytes; ++i)
        b->data[i] = 0xFF;

    size_t extra = bytes * 8 - b->bitcount;
    if (extra) {
        uint8_t mask = (uint8_t)(0xFF >> extra);
        b->data[bytes - 1] &= mask;
    }
}

void bitmap_unset_all(bitmap_t* b)
{
    size_t bytes = bitmap_byte_count(b);

    for (size_t i = 0; i < bytes; ++i)
        b->data[i] = 0x00;
}

void bitmap_copy(bitmap_t* dst, const bitmap_t* src)
{
    size_t dst_bytes = bitmap_byte_count(dst);
    size_t src_bytes = bitmap_byte_count(src);
    size_t min_bytes = (dst_bytes < src_bytes) ? dst_bytes : src_bytes;

    for (size_t i = 0; i < min_bytes; ++i)
        dst->data[i] = src->data[i];

    for (size_t i = min_bytes; i < dst_bytes; ++i)
        dst->data[i] = 0x00;

    size_t extra = dst_bytes * 8 - dst->bitcount;
    if (extra) {
        uint8_t mask = (uint8_t)(0xFF >> extra);
        dst->data[dst_bytes - 1] &= mask;
    }
}

void bitmap_init(bitmap_t* b, size_t bitcount)
{
    if (bitcount > BITMAP_MAX_BYTES * 8)
        bitcount = BITMAP_MAX_BYTES * 8;

    b->bitcount = bitcount;
    bitmap_unset_all(b);
}

#endif