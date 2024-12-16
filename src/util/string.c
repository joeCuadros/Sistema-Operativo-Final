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
// comparar cadenas
int comparar_cadenas(const char *cadena1, const char *cadena2) {
    while (*cadena1 != '\0' && *cadena2 != '\0') {
        if (*cadena1 != *cadena2) {
            return 0;  // Las cadenas son diferentes
        }
        cadena1++;
        cadena2++;
    }
    return (*cadena1 == '\0' && *cadena2 == '\0'); //1 si son reales
}
// comparar subcadenas
int comparar_subcadenas(const char *cadena1, const char *cadena2) {
    while (*cadena1 != '\0' && *cadena2 != '\0') {
        if (*cadena1 != *cadena2) {
            return 0;  // Las cadenas son diferentes
        }
        cadena1++;
        cadena2++;
        if (*cadena1 == '['){
            return 1;
        }
    }
    return 0; 
}


// separar cadena 
void separar_cadena(const char* cadena, char** parte1, char** parte2) {
    static char temp1[50], temp2[50];
    int i = 0, j = 0;

    while (cadena[i] != '\0' && cadena[i] != '[') {
        temp1[i] = cadena[i];
        i++;
    }
    temp1[i] = '\0';  

    if (cadena[i] == '[') {
        i++;

        while (cadena[i] != ']' && cadena[i] != '\0') {
            temp2[j] = cadena[i];
            i++;
            j++;
        }
        temp2[j] = '\0';
    } else {
        temp2[0] = '\0';
    }

    *parte1 = temp1;
    *parte2 = temp2;
}

void asignar_cadena(char *destino, const char *origen) {
    while (*origen != '\0') {  
        *destino = *origen;   
        destino++;             
        origen++;             
    }
    *destino = '\0';  
}

uint32_t convertir_a_entero(const char *str) {
    uint32_t numero = 0;

    if (*str == '-') {
        str++; 
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        numero = numero * 10 + (*str - '0');
        str++;  
    }

    return numero;
}

void limpiar_pantalla() {
    const char* secuencia_escape = "\033[2J\033[H";  // Secuencia para limpiar la pantalla
    while (*secuencia_escape) {
        _putchar(*secuencia_escape);  // Escribe cada carácter de la secuencia
        secuencia_escape++;  // Avanza al siguiente carácter de la secuencia
    }
}