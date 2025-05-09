#include "bajo_nivel.h"

void outb(uint16_t port, uint8_t value){
    __asm__ volatile ("outb %0, %1"::"a"(value),"Nd"(port));
}

uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void io_wait() {
    __asm__ volatile ("outb %%al, $0x80"::"a"(0));
}
// deshabiliar 
void cli_asm(){
    __asm__ volatile("cli");
} 
//habilitar
void sti_asm(){
    __asm__ volatile("sti");
} 
