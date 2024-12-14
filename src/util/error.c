#include "error.h"
#include "../printf/printf.h"

// Imprimir error irrecuperable
void kernel_panic(const char* str){
    printf("Kernel panic: %s\n",str);
    while(1) {
        __asm__ ("hlt"); 
    } 
}
void error_kernel(const uint32_t pid, const char* tipo, const char* str){
    printf("pid [%llx] Error (%s): %s\n",pid,tipo, str);
}