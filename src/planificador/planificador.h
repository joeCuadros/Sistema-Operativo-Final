#ifndef _PLANIFICADOR_H
#define _PLANIFICADOR_H
#include <stdint.h>

#define quantumTotal 5 //round robin Q=5
#define ProcesoTotales 100 // por cada prioridad

// para el timer
struct Pit {
    volatile uint64_t tiempo; // controlar el timer
    volatile uint64_t contador;
    volatile uint8_t quantum;
};

void iniciar_timer(int tiempo);
void aumentar_timer();


#endif