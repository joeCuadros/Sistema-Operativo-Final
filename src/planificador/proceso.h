// NO usar 
#ifndef _PROCESO_H
#define _PROCESO_H
#include <stdint.h>
#include "../printf/printf.h"

// sizeof(Estructura)
struct datos{
    uint16_t numero;
    char mensaje[256];
};
typedef int (*operacion_t)(struct datos*); 

struct proceso{
    uint32_t pid;
    uint8_t prioridad; //0 = interactivo,1 = bash
    uint8_t estado;
    operacion_t operacion;
    void * memoria;
    struct datos *datosProceso;
};

int procesoImprimir100(struct datos* datos);
int procesoInfinto(struct datos* datos);
#endif