#ifndef _BOOT_H
#define _BOOT_H
#include "limine.h" //importar de limine
#include <stdint.h>

char * obtener_bootloader_nombre();
char * obtener_bootloader_version();

uint64_t obtener_contador_terminal();
uint64_t obtener_terminal_actual();

void establecer_terminal(uint64_t terminal);
void (*escribir_terminal())(const char*, uint64_t);
#endif