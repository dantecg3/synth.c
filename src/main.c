#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "input.c"

int main()
{
    bool running = true;
    input_context_t input = {0};
    
    input_init(&input);
    while (running)
    {
        input_update(&input);

        if ((input_is_key_pressed(&input, 'q')))
            break;

        if (input_is_key_pressed(&input, 'a'))
            printf("HELD A!\n");
        
        fflush(stdout);
        usleep(1000);
    }
    input_terminate(&input);

    return 0;
}