#include "planificador.h"
#include "../io/bajo_nivel.h"
#include "../printf/printf.h"

#define PIT_A 0x40
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SET 0x36

// Controlador del timer
#define quantum 5 //round robin Q=5
struct Pit pit;

void colocar_velocidad(int tiempo){
    uint16_t divisor = (1193182/tiempo)/2;
    outb(PIT_CONTROL, PIT_SET);  // Comando para configurar el PIT
    outb(PIT_A, divisor & PIT_MASK);  // Parte baja (8 bits)
    outb(PIT_A, (divisor >> 8) & PIT_MASK);  // Parte alta (8 bits)
} 

void iniciar_timer(int tiempo){
    pit.tiempo = tiempo;
    pit.contador = 0;
    colocar_velocidad(tiempo);
    printf("Se inicializa el timer con %x tiempo\n",tiempo);
}

void reiniciar_timer(){
    pit.contador = 0;
}

void aumentar_timer(){
    pit.contador += 1;
    if (pit.contador % 100 == 0){
        printf("Cantidad %x -",pit.contador); 
    }
}