#include "idt.h"

void set_offset(struct InterruptDescriptor64* entrada, uint64_t direccion){
    entrada->offset_1 = (uint16_t) (direccion  & 0x000000000000ffff);
    entrada->offset_2 = (uint16_t) ((direccion & 0x00000000ffff0000) >> 16);
    entrada->offset_3 = (uint32_t) ((direccion & 0xffffffff00000000) >> 32);
}
uint64_t get_offset(struct InterruptDescriptor64* entrada){
    uint64_t offset = 0;
    offset |= (uint64_t) entrada->offset_1;
    offset |= (uint64_t) entrada->offset_2 << 16;
    offset |= (uint64_t) entrada->offset_3 << 32;

    return offset;
}