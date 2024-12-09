#include "memoria.h"
#include "../bootloader/bootservices.h" 
#include "../printf/printf.h" //para escribir en pantalla
#include "../util/string.h"
#include "../util/error.h" 

#define DIRECCION_A_PAGINA(x) ((uint64_t)(x) >> 12) //desplazamiento de bits 2^12 = 4096

struct uefi_memory_region memoria_trabajo = {
    .type = LIMINE_MEMMAP_MEMORIA_INUTILIZABLE,
    .lenght = 0,
    .base = 0
};

uint64_t ultima_pagina;
uint64_t total_paginas;
uint8_t *region_fisica;

void bloquear_pagina(uint64_t pagina){
    region_fisica[pagina] = 1;
    ultima_pagina = pagina;
}

void desbloquear_pagina(uint64_t pagina){
    region_fisica[pagina] = 0;
}

void liberar_pagina(void* direccion){
    uint64_t direccion_uint64 = (uint64_t) direccion;
    uint64_t direccion_normalizada = (direccion_uint64 - memoria_trabajo.base);

    if (direccion_uint64 < memoria_trabajo.base){
        kernel_panic("Error al intentar liberar pagina fuera de la region de memoria");
    }
    if (region_fisica[DIRECCION_A_PAGINA(direccion_normalizada)] == 0){
        kernel_panic("Se intenta liberar pagina que no esta bloqueada");
    }
    desbloquear_pagina(DIRECCION_A_PAGINA(direccion_normalizada));
    if (region_fisica[DIRECCION_A_PAGINA(direccion_normalizada)] == 1){
        kernel_panic("Fallo al liberar pagina");
    }
}

// solicitar pagina implementar algoritmo 
void * solicitar_pagina (){
    uint64_t primero = ultima_pagina-1;

    while(region_fisica[ultima_pagina]){
        if(ultima_pagina >= total_paginas){
            ultima_pagina = 0;
        }  
        if (ultima_pagina == primero){
            kernel_panic("No hay memoria disponible ");//implementar algoritmo de paginacion
        }
        ultima_pagina++;
    }

    bloquear_pagina(ultima_pagina);
    //verificar (falta mejorar)
    if (region_fisica[ultima_pagina] == 0){
        kernel_panic("Fallo al solicitar pagina");
    }

    return (void*) ((uint8_t*) memoria_trabajo.base + (ultima_pagina * 0x1000));
}

void iniciar_paginas(){
    total_paginas = memoria_trabajo.lenght / 0x1000; //4096 en hexadecimal
    region_fisica = (uint8_t *)memoria_trabajo.base;
    llenar_memoria((void*)region_fisica, 0, total_paginas); 
    for (uint64_t i = 0; i < (total_paginas / 0x1000); i++ ){
        bloquear_pagina(i);
    }
}

//iniciar memoria para la busqueda de almacenamiento
void iniciar_memoria(){
    uint64_t entradas = obtener_memoria_map_entrada();
    printf("Se encontro %x regiones de memoria\n",entradas);
    uint64_t memoria_total = 0;
    uint64_t memoria_disponible = 0;
    // buscar la memoria disponible mas grande utilizable
    for (uint64_t i = 0; i < entradas; i++ ){
        uint64_t tipo = obtener_memoria_map_tipo(i);
        uint64_t lenght = obtener_memoria_map_longitud(i);
        uint64_t base = obtener_memoria_map_base(i);

        memoria_total += lenght;
        if (tipo == LIMINE_MEMMAP_UTILIZABLE){   
            memoria_disponible += lenght;
            if (lenght > memoria_trabajo.lenght){
                memoria_trabajo.type = tipo;
                memoria_trabajo.lenght = lenght;
                memoria_trabajo.base = base;
            }
        }
    }

    printf("Memoria Total %x\nMemoria Dispoible %x\n",memoria_total,memoria_disponible);
    printf("Region memoria de trabajo: base=%x lenght=%x tipo=%d\n",memoria_trabajo.base,memoria_trabajo.lenght,memoria_trabajo.type);
    iniciar_paginas();
}