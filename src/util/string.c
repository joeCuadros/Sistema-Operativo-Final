#include "string.h"
#include "../printf/kernel_print.h"
#include <stdint.h>

//obtener longitud
uint64_t len(const char* str){
    uint64_t len = 0;
    while (str[len] && len < MAX_STRING_SIZE)
        len++;
    return len;
}

//llenar memoria con c caracter
void llenar_memoria(void* ptr, char c, uint64_t size){
    for(uint64_t i=0; i<size; i++){
        ((char*) ptr)[i] = c; //(char*) ptr[i] = c;
    }
}

void limpiar_pantalla() {
    const char* secuencia_escape = "\033[2J\033[H";  // Secuencia para limpiar la pantalla
    while (*secuencia_escape) {
        _putchar(*secuencia_escape);  // Escribe cada carácter de la secuencia
        secuencia_escape++;  // Avanza al siguiente carácter de la secuencia
    }
}