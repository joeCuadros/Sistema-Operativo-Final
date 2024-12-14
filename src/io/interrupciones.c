#include "interrupciones.h"
#include "../printf/printf.h"
#include "../util/string.h"
#include "../util/error.h"
#include "../memoria/memoria.h"
#include "idt.h"

struct idtr idtr;

// evitar advertencia del compilador arget("no-sse", "no-mmx", "no-3dnow")
// Acciones para determinar error 
__attribute__((interrupt, target("no-sse", "no-mmx", "no-3dnow"))) void Excepcion_General(struct marco_interrupciones* marco){
    (void)marco;
    printf("Se detecto una excepcion generica\n");
    while (1);
}

__attribute__((interrupt, target("no-sse", "no-mmx", "no-3dnow"))) void Fallo_pagina(struct marco_interrupciones* marco){
    (void)marco;
    printf("Fallo de pagina \n");
    while (1);
}

__attribute__((interrupt, target("no-sse", "no-mmx", "no-3dnow"))) void double_fault(struct marco_interrupciones* marco){
    (void)marco;
    kernel_panic("El sistema está en un estado irrecuperable");
}


void set_idt_handler(uint64_t direccion, uint8_t entrada_offset, uint8_t tipo, uint8_t selector){
    struct InterruptDescriptor64* interrupcion = (struct InterruptDescriptor64*)(idtr.offset + entrada_offset * sizeof(struct InterruptDescriptor64)); 
    llenar_memoria(interrupcion, 0, sizeof(struct InterruptDescriptor64));
    set_offset(interrupcion, direccion);
    interrupcion->type_attributes = tipo;
    interrupcion->selector = selector;
}

void iniciar_interrupciones(){
    __asm__("cli"); //deshabilitar interrupciones
    idtr.limite = 256 * sizeof(struct InterruptDescriptor64) - 1;
    idtr.offset = (uint64_t) solicitar_marco(0);
    llenar_memoria((void*)idtr.offset, 0, 256 * sizeof(struct InterruptDescriptor64));
    
    for(int i=0; i < 256; i++){
        set_idt_handler((uint64_t)Excepcion_General, i,ITD_TA_Interrupcion,0x28); //0x28 selector de limine    
    }
    // para futura implementaciones https://wiki.osdev.org/Interrupt_Vector_Table
    set_idt_handler((uint64_t)Fallo_pagina,0x0E,ITD_TA_Interrupcion,0x28);
    set_idt_handler((uint64_t)double_fault,0x08,ITD_TA_Interrupcion,0x28);

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__("sti"); //habilitar interrupciones
}