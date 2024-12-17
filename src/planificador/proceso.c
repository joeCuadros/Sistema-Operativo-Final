#include "proceso.h"
#include "../printf/printf.h"
#include "../memoria/memoria.h"
#include "../io/bajo_nivel.h"
extern volatile struct proceso *procesoActual;
extern volatile uint16_t contador_timer;
extern volatile void * memoriaUniversal;

#define NUEVO       0
#define LISTO       1
#define EJECUTANDO  2
#define ESPERANDO   3
#define TERMINADO   4

/*simulando un proceso*/
// proceso que imprime hasta el 100
int procesoImprimir100(struct datos* datos){
    datos->numero++;
    printf("(Proceso %s %d PID (%d)) ",datos->mensaje,datos->numero,procesoActual->pid);
    if (datos->numero < 100) {return -1;}
    return 0;
}
// proceso que imprime hasta el 100
int procesoImprimir_0(struct datos* datos){
    datos->numero--;
    printf("(Proceso %s %d PID=%d)\n",datos->mensaje,datos->numero,procesoActual->pid);
    if (datos->numero > 0) { return -1;}
    printf("(Proceso terminado %s PID=%d)\n",datos->mensaje,procesoActual->pid);
    return 0;
}
// proceso fantasma
int procesoInfinto(struct datos* datos){
    datos->numero++;
    return -1;
}
//llenar memoria
int procesollenarMemoria(struct datos* datos){
    datos->numero--;
    solicitar_pagina(procesoActual->pid);
    if (datos->numero > 0) { return -1;}
    printf("Termino proceso PID (%d)  %d ",procesoActual->pid,contador_timer);
    return 0;
}
