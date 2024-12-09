#include "error.h"
#include "../printf/printf.h"

// Imprimir error irrecuperable
void kernel_panic(const char* str){
    printf("Kernel panic: %s\n",str);
    while(1) {
        __asm__ ("hlt"); 
    } 
}