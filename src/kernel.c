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
    printf("A mimir zzzz \n");
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
    struct datos datos1= {21,"AA"};
    agregarProceso(procesoInfinto,&datos1,1);
    agregarProceso(procesoImprimir100,&datos1,1);
    struct datos datos2= {20,"BB"};
    agregarProceso(procesoImprimir100,&datos2,1);
    scheduler_RR(); // ejecutar
    struct datos datos3= {31,"CC"};
    printf("Agregar proceso 3 =");
    agregarProceso(procesoImprimir100,&datos3,1);
    descansar(); //dormir
}

