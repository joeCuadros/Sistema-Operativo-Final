#ifndef _TECLADO_H
#define _TECLADO_H
#define ASCII_SIZE 56

#define Izq_Shift 0x2A
#define Der_Shift 0x36
#define Enter 0x1C
#define Backspace 0x0E
#define Space 0x39

#include <stdint.h>

struct Teclado {
    char * ASCII_tabla;
    uint8_t Izq_Shift_estado; //1 presionado
    uint8_t Der_Shift_estado;
    uint8_t enter_teclado;
};

void iniciar_teclado();
char codigo_caracter(uint8_t codigo, uint8_t mayuscula);
void recibir_codigo(uint8_t codigo);

// habilitar y deshabilitar
void habilitar_teclado(void (*evento_teclado)(char));
void deshabilitar_teclado();
void esperar_enter();

#endif
