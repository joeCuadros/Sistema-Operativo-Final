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
    
    void * pagina1 = solicitar_pagina();
    printf("Pagina 1: %p\n", pagina1);
    void * pagina2 = solicitar_pagina();
    printf("Pagina 2: %p\n", pagina2);
    liberar_pagina(pagina2);
    printf("Se libero pagina 2");
    void * pagina3 = solicitar_pagina();
    printf("Pagina 1: %p\n", pagina3);
    printf("Hola mundo desde DumpOS\n");
    descansar();
}