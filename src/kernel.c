#include "bootloader/bootservices.h"
#include "printf/printf.h" 
#include "memoria/memoria.h"
#include "io/interrupciones.h"

void descansar(){
    while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}

void _start() {   
    iniciar_memoria();
    iniciar_interrupciones();
    
    printf("Hola mundo desde DumpOS\n");
    descansar();
}

/* Para probar error 
uint64_t * ptr = (uint64_t*)0xffffffffdeadb000;
*ptr = 0xdeadbeef;*/