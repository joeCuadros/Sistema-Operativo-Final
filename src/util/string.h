#ifndef _STRING_H
#define _STRING_H
#define MAX_STRING_SIZE 655536
#include <stdint.h>

uint64_t len(const char* str);
void llenar_memoria(void* ptr, char c, uint64_t size);
int comparar_cadenas(const char *cadena1, const char *cadena2);
int comparar_subcadenas(const char *cadena1, const char *cadena2);
void separar_cadena(const char* cadena, char** parte1, char** parte2);
void asignar_cadena(char *destino, const char *origen);
void limpiar_pantalla();
uint32_t convertir_a_entero(const char *str);

#endif