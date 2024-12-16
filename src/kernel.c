#include "bootloader/bootservices.h"
#include "printf/printf.h" 
#include "memoria/memoria.h"
#include "io/interrupciones.h"
#include "util/string.h"
#include "util/error.h"
#include "planificador/planificador.h"
#include "planificador/proceso.h"
#include "entrada/teclado.h"

void descansar(){
    printf("A mimir zzzz \n");
    while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}
// de prueba
int procesoX(struct datos* datos){
    datos->numero++;
    printf("(P %s %d) -> ",datos->mensaje,datos->numero);
    if (datos->numero < 100) {return -1;}
    return 0;
}
int procesoY(struct datos* datos){
    datos->numero++;
    printf("(P %s %d) -> ",datos->mensaje,datos->numero);
    if (datos->numero < 10) {return -1;}
    return 0;
}
int shell(struct datos* datos){
    printf("> %s ",datos->mensaje);
    habilitar_teclado(manejar_teclado);
    datos->numero = 0;
    return 1;
}

void _start() {   
    iniciar_memoria();
    iniciar_planificador();
    iniciar_interrupciones();
    
    // colocar
    limpiar_pantalla();
    printf("Ejecutando despues la configuracion inicial\n");
    printf("-------------------------------------------\n");
    operacion_t operador = procesoX;
    struct datos datos1= {1,"AA"};
    agregarProceso(operador,&datos1,1);
    struct datos datos2= {50,"BB"};
    agregarProceso(operador,&datos2,1);
    struct datos datosX= {0,"Joe"};
    agregarProceso(shell,&datosX,0);
    scheduler();
    struct datos datos3= {1,"CC"};
    printf("Agregar proceso 3 =");
    agregarProceso(procesoY,&datos3,1);
    printf("-------------------------------------------\n");    
    descansar(); //dormir
}




/* Para probar error 
uint64_t * ptr = (uint64_t*)0xffffffffdeadb000;
*ptr = 0xdeadbeef;
copiar_bloque(&recuperar_dato,pagina,MAXIMO_pagina);
// esatablecer 
uint8_t primer_bit = (var >> 7) & 0x01; //extraer bit
var |= (1 << 7);  // 1
var &= ~(1 << 7); // 0
    printf(" Direccion de al liberar pagina 0x%llx\n",DIRECCION_A_pagina(direccion_normalizada));
*/
// probando marco
/*
uint32_t pid1 = 1;
    uint32_t pid2 = 2;
    uint32_t pid3 = 3;
    void * pagina = solicitar_pagina(pid1); 
    void * pagina2 = solicitar_pagina(pid2);
    ver_permisos(pagina);
    struct datos datos2 = {1,154};
    escribir_pagina(pagina,&datos2,pid1);
    struct datos nuevoDatos;
    struct permisos perm = {1,1,1};
    set_demas(pagina,perm,pid1);
    set_demas(pagina,perm,pid3);
    ver_permisos(pagina);
    leer_pagina(&nuevoDatos,pagina,pid2);
    printf("id: %u\nvalor: %u\n", nuevoDatos.id, nuevoDatos.valor); 
    ver_permisos(pagina2);
*/