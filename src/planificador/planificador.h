#ifndef _PLANIFICADOR_H
#define _PLANIFICADOR_H
#include <stdint.h>
#include "proceso.h"
#define quantumTotal 5 //round robin Q=5
#define ProcesoTotales 10 // por cada prioridad

// para el timer
struct Pit {
    volatile uint64_t tiempo; // controlar el timer
    volatile uint64_t contador;
    volatile uint8_t quantum;
};

void iniciar_timer(int tiempo);
void aumentar_timer();
void iniciar_planificador();
void scheduler_RR();
void agregarProceso(operacion_t operacion, struct datos *datos, uint8_t prioridad);
void listaProcesos();
void eliminarProceso(uint32_t pid);
#endif