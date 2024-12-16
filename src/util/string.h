#ifndef _STRING_H
#define _STRING_H
#define MAX_STRING_SIZE 655536
#include <stdint.h>

uint64_t len(const char* str);
void llenar_memoria(void* ptr, char c, uint64_t size);
void limpiar_pantalla();

#endif