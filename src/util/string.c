#include "string.h"
#include <stdint.h>

uint64_t strlen(const char* str){
    uint64_t len = 0;
    while (str[len] && len < MAX_STRING_SIZE)
        len++;
    return len;
}