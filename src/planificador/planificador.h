#ifndef _PLANIFICADOR_H
#define _PLANIFICADOR_H
#include <stdint.h>

struct Pit {
    uint64_t tiempo; // controlar el timer
    volatile uint64_t contador;
};

void iniciar_timer(int tiempo);
void aumentar_timer();
#endif