#include "bootloader/bootservices.h"
#include "printf/printf.h" 
#include "memoria/memoria.h"

void descansar(){
     while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}

void _start() {   
    iniciar_memoria();
    printf("Hola mundo desde DumpOS\n");
    descansar();
}