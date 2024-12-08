#include "bootservices.h"
#include "limine_request.h"

#define LLAMAR_SERVICIO(x) (x.response) //solo mandar serivicios puestos

#define TERMINAL_PREDETERMINADA 0x0
uint64_t terminal_actual = TERMINAL_PREDETERMINADA;

//no usado desde afuera 
void escribir_terminal_generico(const char* str, uint64_t length){ 
    struct limine_terminal * terminal = LLAMAR_SERVICIO(TERMINAL)->terminals[terminal_actual];
    LLAMAR_SERVICIO(TERMINAL)->write(terminal,str,length);
}
void error_servicio(){
    while(1) {
        __asm__ ("hlt"); //dormir cpu
    }
}

// Codigo a usar 
char * obtener_bootloader_nombre(){
    return LLAMAR_SERVICIO(BOOTLOADER)->name;
}

char * obtener_bootloader_version(){
    return LLAMAR_SERVICIO(BOOTLOADER)->version;
}

uint64_t obtener_contador_terminal(){
    return LLAMAR_SERVICIO(TERMINAL)->terminal_count;
}

uint64_t obtener_terminal_actual(){
    return terminal_actual;
}

void establecer_terminal(uint64_t terminal){
    if (terminal > obtener_contador_terminal()){
        error_servicio(); //invocar error
    }
    terminal_actual = terminal;
}
void (*escribir_terminal())(const char*, uint64_t){
    return escribir_terminal_generico;
}

