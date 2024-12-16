#include "bootloader/bootservices.h"
#include "printf/printf.h" 
#include "memoria/memoria.h"
#include "io/interrupciones.h"
#include "util/string.h"
#include "util/error.h"
#include "planificador/planificador.h"
#include "planificador/proceso.h"
#include "entrada/teclado.h"
#include "shell/shell.h"

void descansar(){
    while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}
void ejecutar_kernel_exp();
void _start() {   
    __asm__ ("cli"); // deshabilitar
    iniciar_memoria();
    iniciar_planificador();
    __asm__ ("sti"); //habilitar
    iniciar_interrupciones();
    
    // colocar
    printf("Ejecutando despues la configuracion inicial\n");
    printf("-------------------------------------------\n");
    struct datos datosX= {0,"Joe"};
    agregarProceso(shell_comandos,&datosX,0);  
    scheduler_RR(); // ejecutar
    descansar(); //dormir
}
