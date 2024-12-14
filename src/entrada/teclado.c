#include "teclado.h"
#include "../memoria/memoria.h"
#include "../util/string.h"
#include "../printf/printf.h"
#define MAXIMO_MARCO 4096

volatile struct Teclado* teclado;
void (*evento_teclado)(uint8_t);

char asciitabla[] = {
    0,   0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void iniciar_teclado(){
    teclado = (struct Teclado *) solicitar_marco(0);
    teclado->ASCII_tabla = asciitabla;
    // estados iniciales
    teclado->Izq_Shift_estado = 0;
    teclado->Der_Shift_estado = 0;
    teclado->enter_teclado = 0;
    evento_teclado = 0;
}

char codigo_caracter(uint8_t codigo, uint8_t mayuscula){
    if (codigo > 58 ) return 0;
    if (mayuscula) return teclado->ASCII_tabla[codigo]-32;
    return teclado->ASCII_tabla[codigo];
}

void recibir_codigo(uint8_t codigo){
    switch(codigo){
        case Space:
            printf(" ");
            return;
        case Izq_Shift:
            teclado->Izq_Shift_estado = 1;
            return;
        case Izq_Shift + 0x80:
            teclado->Izq_Shift_estado = 0;
            return;
        case Der_Shift:
            teclado->Der_Shift_estado = 1;
            return;
        case Der_Shift + 0x80:
            teclado->Der_Shift_estado = 0;
            return;
        case Enter:
            teclado->enter_teclado = 1;
            printf("\n");
            if (evento_teclado != 0) evento_teclado(Enter); 
            return;
        case Backspace:
            printf("\b \b");
            if (evento_teclado != 0) evento_teclado(Backspace); 
            return;   
    }
    char caracter = codigo_caracter(codigo, teclado->Der_Shift_estado || teclado->Izq_Shift_estado);
    if (caracter != 0){
        printf("%c", caracter); 
        if (evento_teclado != 0) evento_teclado(caracter);
    }
}

void esperar_enter();
void habilitar_teclado(void (*funsion_teclado)(uint8_t)){
    evento_teclado = funsion_teclado;
}
// deshabilitar teclado
void deshabilitar_teclado(){
    evento_teclado = 0; 
}
// espera activa hasta que haga caso
void esperar_enter(){
    teclado->enter_teclado = 0;
    printf("Presiona enter para continuar ...");
    while (1) {
        if (teclado->enter_teclado) {
            teclado->enter_teclado = 0;
            return;
        }
    }
}