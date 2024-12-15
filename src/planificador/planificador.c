#include "planificador.h"
#include "../io/bajo_nivel.h"
#include "../printf/printf.h"
#include "proceso.h"
#include "../memoria/memoria.h"
#define PIT_A 0x40
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SET 0x36

#define NUEVO       0
#define LISTO       1
#define EJECUTANDO  2
#define ESPERANDO   3
#define TERMINADO   4

struct Pit pit; //timer
struct proceso procesos[ProcesoTotales]; //RR Q = 5
volatile uint64_t actual = 0; 
uint32_t proceso_colocar = 0; 
uint32_t pidDisponible = 1; // 0 es kernel 
volatile int flag_interrupcion = 0; //evitar optimizacion del compilador
volatile uint8_t restaurando = 0; //0 falso 1 verdadero

// Controlador del timer
void colocar_velocidad(int tiempo){
    uint16_t divisor = (1193182/tiempo)/2;
    outb(PIT_CONTROL, PIT_SET);  // Comando para configurar el PIT
    outb(PIT_A, divisor & PIT_MASK);  // Parte baja (8 bits)
    outb(PIT_A, (divisor >> 8) & PIT_MASK);  // Parte alta (8 bits)
} 

void iniciar_timer(int tiempo){
    pit.tiempo = tiempo;
    pit.contador = 0;
    pit.quantum = 0;
    colocar_velocidad(tiempo);
    printf("Se inicializa el timer con %x tiempo\n",tiempo);
}

void reiniciar_timer(){
    pit.contador = 0;
    pit.quantum = 0;
}
/*
// planificador metodos
void ejecutarProceso() {
    struct proceso *proceso = &procesos[actual];
    guardar_contexto(&contexto_general); //guardar cpu
    if (flag_interrupcion == 0){
        if (proceso->contexto == NULO){ // si no hay contexto
            int resultado = proceso->operacion(proceso->memoria); //ejecutar programa
            if (resultado == 0) { // termino bien
                reiniciar_timer(); 
                proceso->estado = TERMINADO; 
                procesos->contexto = NULO;
            }
        }else{
            restaurando = 1;
            restaurar_contexto(proceso->contexto);
        }      
    } else{//interrupcion por reloj
        flag_interrupcion = 0;
    }

}

void cambio_de_contexto(){
    reiniciar_timer();
    struct proceso *proceso = &procesos[actual];
    proceso->estado = LISTO;
    guardar_contexto(proceso->contexto);
    // si el proceso regresa ejecuta como estaba
    if (restaurando){
        restaurando = 0;
    }else{
        escribir_pagina(proceso->memoria,proceso->datosProceso,proceso->pid); //guardar en memoria
        flag_interrupcion = 1; // para realizar Cambio de contexto
        restaurar_contexto(&contexto_general);
    }
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
    procesoNuevo->datosProceso = datos;  
    
    pidDisponible++; //para el siguente proceso
}

// planificador
void round_robin_scheduler(){
    while(1) {
        struct proceso* proceso = &procesos[actual];
        if (proceso->estado == LISTO){
            proceso->estado = EJECUTANDO;
            ejecutarProceso(proceso->operacion,proceso->memoria);
        }
        actual = (actual + 1) % ProcesoTotales;
    }
}

void iniciar_planificador(){
    for (uint16_t i=0; i < ProcesoTotales; i++){
        procesos[i].estado = NUEVO; 
        procesos[i].contexto = NULO;
    }
}
*/
// timer 
void aumentar_timer(){
    pit.contador += 1;
    if (pit.contador % 100 == 0){ //ajutar tiempo por cada Quantum 
        pit.quantum += 1;
        if (pit.quantum == quantumTotal){
            //cambio_de_contexto();
            printf("Tiempo "); 
        }
        printf("%x Quatum ",pit.quantum); 
    }
}
