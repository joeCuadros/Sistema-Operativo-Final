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

void _start() {   
    iniciar_memoria();
    iniciar_planificador();
    iniciar_interrupciones();
    
    // colocar
    printf("Ejecutando despues la configuracion inicial\n");
    printf("-------------------------------------------\n");
    struct datos datosX= {0,"Joe"};
    agregarProceso(shell_comandos,&datosX,0);  
    scheduler_RR(); // ejecutar
    
   
    descansar(); //dormir
}

/*
struct datos datos1= {10,"A"};
agregarProceso(procesoInfinto,&datos1,1);
//agregarProceso(procesoInfinto,&datos1,1);
struct datos datos2= {20,"B"};
agregarProceso(procesoImprimir_0,&datos2,1);
struct datos datos3= {31,"C"};
agregarProceso(procesoImprimir_0,&datos3,1);
*/
