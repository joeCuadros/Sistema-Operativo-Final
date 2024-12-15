#ifndef _BAJO_NIVEL_H
#define _BAJO_NIVEL_H
#include <stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void io_wait();
void cli_asm();
void sti_asm();
#endif