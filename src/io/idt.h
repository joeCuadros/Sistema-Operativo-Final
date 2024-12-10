#ifndef _IDT_H
#define _IDT_H
#include <stdint.h>

#define ITD_TA_Interrupcion 0x8E
#define ITD_TA_LLamar       0x8C
#define ITD_TA_Trap         0x8F

//Tabla de descriptores de interrupciones de 64 bits
struct InterruptDescriptor64 {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t zero;            // reserved
};

struct idtr{
    uint16_t limite;
    uint64_t offset;
} __attribute__((packed));

void set_offset(struct InterruptDescriptor64* entrada, uint64_t direccion);
uint64_t get_offset(struct InterruptDescriptor64* entrada);

#endif