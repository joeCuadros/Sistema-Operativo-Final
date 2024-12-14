#include "interrupciones.h"
#include "../printf/printf.h"
#include "../util/string.h"
#include "../util/error.h"
#include "../memoria/memoria.h"
#include "idt.h"
#include "bajo_nivel.h"
#include "../entrada/teclado.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct idtr idtr;

void pic_end_master(){
	outb(PIC1_COMMAND, PIC_EOI);
}

void pic_end_slave(){
    outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}
void remap_pic(){
	uint8_t a1, a2;
	a1 = inb(PIC1_DATA);
	io_wait();
	a2 = inb(PIC2_DATA);
	io_wait();
	
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	
	outb(PIC1_DATA,0x20);
	io_wait();
	outb(PIC2_DATA,0x20);
	io_wait();

	outb(PIC1_DATA,0x04);
	io_wait();
	outb(PIC2_DATA,0x02);
	io_wait();

	outb(PIC1_DATA,ICW4_8086);
	io_wait();
	outb(PIC2_DATA,ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	io_wait();
	outb(PIC2_DATA, a2);
	io_wait();
}
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

__attribute__((interrupt, target("no-sse", "no-mmx", "no-3dnow"))) void Int_teclado(struct marco_interrupciones* marco){
    (void)marco;
    uint8_t codigo = inb(0x60);
    recibir_codigo(codigo);
    pic_end_master();
}

void set_idt_handler(uint64_t direccion, uint8_t entrada_offset, uint8_t tipo, uint8_t selector){
    struct InterruptDescriptor64* interrupcion = (struct InterruptDescriptor64*)(idtr.offset + entrada_offset * sizeof(struct InterruptDescriptor64)); 
    //llenar_memoria(interrupcion, 0, sizeof(struct InterruptDescriptor64));
    set_offset(interrupcion, direccion);
    interrupcion->type_attributes = tipo;
    interrupcion->selector = selector;
}

void iniciar_interrupciones(){
    __asm__("cli"); //deshabilitar interrupciones
    printf("Iniciando las interrupciones \n");
    idtr.limite = 256 * sizeof(struct InterruptDescriptor64) - 1;
    idtr.offset = (uint64_t) solicitar_marco(0);
    llenar_memoria((void*)idtr.offset, 0, 4096);
    /*
    for(int i=0; i < 256; i++){
        set_idt_handler((uint64_t)Excepcion_General, i,ITD_TA_Interrupcion,0x28); //0x28 selector de limine    
    }*/
    // para futura implementaciones https://wiki.osdev.org/Interrupt_Vector_Table
    set_idt_handler((uint64_t)Fallo_pagina,0x0E,ITD_TA_Interrupcion,0x28);
    set_idt_handler((uint64_t)double_fault,0x08,ITD_TA_Interrupcion,0x28);
    set_idt_handler((uint64_t)Int_teclado,0x21,ITD_TA_Interrupcion,0x28);
    __asm__ volatile("lidt %0" : : "m"(idtr));
    remap_pic();
    iniciar_teclado();
    outb(PIC1_DATA,0xf9);
	outb(PIC2_DATA,0xef);
    printf("Terminando las interrupciones \n");
    __asm__("sti"); //habilitar interrupciones
}