#include "planificador.h"
#include "../io/bajo_nivel.h"
#include "../printf/printf.h"
#include "proceso.h"
#include "../memoria/memoria.h"
#include "../util/string.h"
#define PIT_A 0x40
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SET 0x36

#define NUEVO       0
#define LISTO       1
#define EJECUTANDO  2
#define ESPERANDO   3
#define TERMINADO   4

struct Pit *pit;//timer
struct proceso procesos[ProcesoTotales]; //RR Q = 5
volatile uint64_t actual; 
volatile uint32_t proceso_colocar = 0; 
volatile uint32_t pidDisponible = 1; // 0 es kernel 
volatile uint8_t restaurando = 0; //0 falso 1 verdadero
volatile uint16_t procesos_listos;
volatile struct proceso * procesoActual;

// Controlador del timer
void colocar_velocidad(int tiempo){
    uint16_t divisor = (1193182/tiempo)/2 ;
    outb(PIT_CONTROL, PIT_SET);  // Comando para configurar el PIT
    outb(PIT_A, divisor & PIT_MASK);  // Parte baja (8 bits)
    outb(PIT_A, (divisor >> 8) & PIT_MASK);  // Parte alta (8 bits)
} 

void iniciar_timer(int tiempo){
    pit->tiempo = tiempo;
    pit->contador = 0;
    pit->quantum = 0;
    colocar_velocidad(tiempo);
    printf("Se inicializa el timer con %x tiempo\n",tiempo);
}


int ejecutarProceso() {
    int resultado = procesoActual->operacion(procesoActual->datosProceso);
    return resultado;
}
void agregarProceso(operacion_t operacion, struct datos *datos, uint8_t prioridad){
    uint32_t inicio = proceso_colocar;
    // buscar un nuevo 
    while (procesos[proceso_colocar].estado != NUEVO){
        if (inicio == proceso_colocar){
            printf("No se puede colocar el proceso\n");
            return;
        }
        proceso_colocar = (proceso_colocar + 1) % ProcesoTotales;
    }
    struct proceso *procesoNuevo = &procesos[proceso_colocar]; 
    procesoNuevo->pid=pidDisponible;
    procesoNuevo->prioridad = prioridad;
    procesoNuevo->estado=LISTO;
    procesoNuevo->operacion = operacion;
    procesoNuevo->datosProceso = (struct datos *)solicitar_pagina(procesoNuevo->pid);  
    *(procesoNuevo->datosProceso) = *datos;
    proceso_colocar = (proceso_colocar + 1) % ProcesoTotales;
    procesos_listos++;
    pidDisponible++; //para el siguente proceso
}

// planificador buscar proceso
void scheduler(){
    // buscar proceso
    if (procesos_listos == 0) {
        printf("No hay procesos listos\n");
        return;
    }
    uint64_t inicio = actual;
    actual = (actual + 1) % ProcesoTotales; //buscar siguente proceso
    while (procesos[actual].estado != LISTO){
        actual = (actual + 1) % ProcesoTotales;
        if(inicio == actual){
            printf("np_eje");
            return;
        }
    }
    procesoActual = &procesos[actual];
}



void iniciar_planificador(){
    pit = (struct Pit *) solicitar_pagina(0);
    for (uint16_t i=0; i < ProcesoTotales; i++){
        procesos[i].estado = NUEVO; 
    }
    pit->contador = 0;
    pit->quantum = 0;
    pit->tiempo = 0;
    procesos_listos = 0;
    actual = 0;
}

// planificador
void aumentar_timer(){
    pit->contador += 1;
    if (pit->contador % 10 == 0 && procesos_listos != 0){ //ajutar tiempo por cada Quantum 
        pit->quantum += 1;
        int resultado = ejecutarProceso(); //ejecutar 1 
        if (resultado == 0){
            procesoActual->estado = TERMINADO;
            procesos_listos--; 
            scheduler(); //invocar planificador
            return;
        }else if (resultado == 1){
            procesoActual->estado = ESPERANDO;
            procesos_listos--; 
            scheduler(); //invocar planificador
            return;
        }
        if (pit->quantum == quantumTotal && resultado == -1){
            printf("[Cambio contexto]");
            procesoActual->estado = LISTO;
            pit->quantum = 0;
            scheduler(); //invocar planificador
            return;
        }
    }
}
