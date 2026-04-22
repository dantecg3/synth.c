#ifndef STRING_IMPL
#define STRING_IMPL

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {
    #define STRING_MAX_CHARS 512
    char data[STRING_MAX_CHARS];
    size_t length;
} string_t;

#define NULL_STRING (string_t){0}

typedef struct {
    const char* data;
    size_t length;
} string_view_t;

string_t string_ctr(const char *str)
{
    if (!str) return NULL_STRING;

    string_t s = {0};
    
    size_t i = 0;
    while (str[i] != '\0' && i < STRING_MAX_CHARS - 1) {
        s.data[i] = str[i];
        i++;
    }

    s.data[i] = '\0';
    s.length = i;

    return s;
}

void string_append(string_t *string, const char *str)
{
    if (!string || !str)
        return;

    size_t i = 0;
    while (str[i] && string->length < STRING_MAX_CHARS - 1) {
        string->data[string->length] = str[i];
        string->length++;
        i++;
    }

    string->data[string->length] = '\0';
}

void string_format(string_t *string, const char *fmt, ...)
{
    if (!string || !fmt)
        return;

    va_list args;
    va_start(args, fmt);

    int written = vsnprintf(string->data, STRING_MAX_CHARS, fmt, args);

    va_end(args);

    if (written < 0) {
        string->length = 0;
        string->data[0] = '\0';
        return;
    }

    if (written >= STRING_MAX_CHARS)
        written = STRING_MAX_CHARS - 1;

    string->length = (size_t)written;
}

string_view_t string_view(const char* str)
{
    string_view_t v = {0};

    if (!str)
        return v;

    size_t i = 0;
    while (str[i])
        i++;

    v.data = str;
    v.length = i;

    return v;
}

#endif