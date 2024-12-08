#include "kernel_print.h"
#include "../bootloader/bootservices.h"
#include <stdint.h>

void _putchar(char character)
{
    void (*writer)(const char*, uint64_t) = escribir_terminal();
    writer(&character, 1);
}

