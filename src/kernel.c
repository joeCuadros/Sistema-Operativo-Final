#include "bootloader/bootservices.h"
#include "printf/printf.h" 
#include "memoria/memoria.h"
#include "io/interrupciones.h"
#include "util/string.h"
#include "util/error.h"
#define MAXIMO_MARCO 4096

void descansar(){
    printf("A mimir zzzz \n");
    while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}

struct datos {
    uint8_t id;
    uint8_t valor;
};

void _start() {   
    iniciar_memoria();
    iniciar_interrupciones();
    // colocar
    printf("Ejecutando despues la configuracion inicial\n");
    /*
    uint32_t pid1 = 1;
    uint32_t pid2 = 2;
    uint32_t pid3 = 3;
    void * marco = solicitar_marco(pid1); 
    void * marco2 = solicitar_marco(pid2);
    ver_permisos(marco);
    struct datos datos2 = {1,154};
    escribir_marco(marco,&datos2,pid1);
    struct datos nuevoDatos;
    struct permisos perm = {1,1,1};
    set_demas(marco,perm,pid1);
    set_demas(marco,perm,pid3);
    ver_permisos(marco);
    leer_marco(&nuevoDatos,marco,pid2);
    printf("id: %u\nvalor: %u\n", nuevoDatos.id, nuevoDatos.valor); 
    ver_permisos(marco2);
    */
    descansar(); //dormir
}

/* Para probar error 
uint64_t * ptr = (uint64_t*)0xffffffffdeadb000;
*ptr = 0xdeadbeef;
copiar_bloque(&recuperar_dato,marco,MAXIMO_MARCO);
// esatablecer 
uint8_t primer_bit = (var >> 7) & 0x01; //extraer bit
var |= (1 << 7);  // 1
var &= ~(1 << 7); // 0
    printf(" Direccion de al liberar marco 0x%llx\n",DIRECCION_A_marco(direccion_normalizada));
*/