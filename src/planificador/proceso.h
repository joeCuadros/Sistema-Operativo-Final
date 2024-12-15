// NO usar 
#ifndef _PROCESO_H
#define _PROCESO_H
#include <stdint.h>

// sizeof(Estructura)
struct datos{
    uint16_t longitud;
    char *mensaje;
};
typedef int (*operacion_t)(struct datos*); 

struct proceso{
    uint32_t pid;
    uint8_t prioridad; //1 = interactivo, bash
    uint8_t estado;
    operacion_t operacion;
    void * memoria;
    struct datos *datosProceso;
};


#endif