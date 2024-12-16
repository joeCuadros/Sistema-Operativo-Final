#include "memoria.h"
#include "../bootloader/bootservices.h" 
#include "../printf/printf.h" //para escribir en pantalla
#include "../util/string.h"
#include "../util/error.h" 
#define MAXIMO_pagina 4096

#define DIRECCION_A_pagina(x) ((uint64_t)(x) >> 12) //desplazamiento de bits 2^12 = 4096

struct uefi_memory_region memoria_trabajo = {
    .type = LIMINE_MEMMAP_MEMORIA_INUTILIZABLE,
    .lenght = 0,
    .base = 0
};

uint64_t ultimo_pagina_RAM;
uint64_t total_paginas;
uint64_t total_paginas_RAM;
uint64_t total_paginas_DISCO;
struct pagina_info *region_memoria_RAM;
struct pagina_info *region_memoria_DISCO;

// region_memoria_RAM --------------------------------------------------------------------------------------------
void bloquear_pagina(uint64_t pagina){
    region_memoria_RAM[pagina].estado = 1;
    ultimo_pagina_RAM = pagina;
}

void desbloquear_pagina(uint64_t pagina){
    region_memoria_RAM[pagina].estado = 0;
}

// copiar y escribir
void copiar_bloque(const void* origen, void* destino, uint64_t n) {
    uint8_t* d = (uint8_t*)destino;
    const uint8_t* o = (const uint8_t*)origen;

    for (uint64_t i = 0; i < n; i++) {
        d[i] = o[i];
    }
}

uint64_t normalizar (uint64_t direccion_uint64){
    if (direccion_uint64 < memoria_trabajo.base){
        kernel_panic("Error al intentar liberar pagina fuera de la region de memoria");
    }
    uint64_t direccion_normalizada = (direccion_uint64 - memoria_trabajo.base);
    return DIRECCION_A_pagina(direccion_normalizada);
}

// eliminacion logico del recurso 
void liberar_pagina(void* direccion, uint32_t pid){
    uint64_t direccion_uint64 = (uint64_t) direccion;
    uint64_t indice_pagina = normalizar(direccion_uint64);
    if (region_memoria_RAM[indice_pagina].pid != pid){
        error_kernel(pid,"memoria","No se puede liberar pagina que no tienes permiso");
        return ;
    }

    if (region_memoria_RAM[indice_pagina].estado == 0){
        error_kernel(pid,"memoria","Se intenta liberar pagina que no esta bloqueada");
        return ;
    }
    desbloquear_pagina(indice_pagina);
    if (region_memoria_RAM[indice_pagina].estado == 1){
        kernel_panic("Fallo al liberar pagina");
    }
}

// solicitar pagina nuevo (NO OLVIDAR)
void * solicitar_pagina (uint32_t pid){
    uint64_t primero = ultimo_pagina_RAM-1;

    while(region_memoria_RAM[ultimo_pagina_RAM].estado){ // 1 = true, 0 = false
        if(ultimo_pagina_RAM >= total_paginas_RAM){
            ultimo_pagina_RAM = 0;
        }  
        if (ultimo_pagina_RAM == primero){
            kernel_panic("No hay memoria disponible ");//implementar algoritmo de recolocacion (NO OLVIDAR)
        }
        ultimo_pagina_RAM++;
    }

    bloquear_pagina(ultimo_pagina_RAM);
    region_memoria_RAM[ultimo_pagina_RAM].pid = pid;

    if (region_memoria_RAM[ultimo_pagina_RAM].estado == 0){
        kernel_panic("Fallo al solicitar pagina");
    }

    return (void*) ((uint8_t*) memoria_trabajo.base + (ultimo_pagina_RAM * 0x1000));
}
void estado_memoria_RAM(uint32_t verTotal) {
    uint32_t i = 0, activas = 0;

    while (i < total_paginas_RAM && activas < verTotal) {  
        if (region_memoria_RAM[i].estado == 1) {  
            printf("Region de memoria con pid: %d\n", region_memoria_RAM[i].pid); 
            activas++;
        }
        i++;
    }
}
void estado_memoria_RAM_pid(uint32_t pid){
    uint32_t i = 0;

    while (i < total_paginas_RAM) {  
        if (region_memoria_RAM[i].pid == pid) {  
            printf("Region de memoria con pid: %d estado(%d)\n", region_memoria_RAM[i].pid, region_memoria_RAM[i].estado); 
        }
        i++;
    }
}
void paginasUsadas(){
    uint32_t i = 0, activas = 0;

    while (i < total_paginas_RAM) {  
        if (region_memoria_RAM[i].estado == 0) {  
            activas++;
        }
        i++;
    }
    printf("%d / %d Paginas Disponibles\n",activas,total_paginas_RAM);
}
// region_memoria_DISCO --------------------------------------------------------------------------------------------



// Configuracion inicial --------------------------------------------------------------------------------------------
// iniciar paginas de memoria
void iniciar_paginas(){
    total_paginas = memoria_trabajo.lenght / 0x1000; //4096 en hexadecimal
    total_paginas_RAM = total_paginas / 2;
    total_paginas_DISCO = total_paginas - total_paginas_RAM;
    printf("Total de paginas %x\n",total_paginas);
    printf("Total de paginas RAM %x\n",total_paginas_RAM);
    printf("Total de paginas DISCO %x\n",total_paginas_DISCO);

    region_memoria_RAM = (struct pagina_info *)memoria_trabajo.base;
    llenar_memoria((void*)region_memoria_RAM, 0, total_paginas_RAM); 
    for (uint64_t i = 0; i < (total_paginas_RAM / 0x1000); i++ ){
        bloquear_pagina(i);
    }
}

//iniciar memoria para la busqueda de almacenamiento
void iniciar_memoria(){
    uint64_t entradas = obtener_memoria_map_entrada();
    printf("Se encontro %x regiones de memoria\n",entradas);
    printf("Memoria utilizable \n");
    uint64_t memoria_total = 0;
    uint64_t memoria_disponible = 0;
    uint64_t memoria_kernel = 0;
    // buscar la memoria disponible mas grande utilizable
    for (uint64_t i = 0; i < entradas; i++ ){
        uint64_t tipo = obtener_memoria_map_tipo(i);
        uint64_t lenght = obtener_memoria_map_longitud(i);
        uint64_t base = obtener_memoria_map_base(i);
        memoria_total += lenght;
        if (tipo == LIMINE_MEMMAP_UTILIZABLE){ 
            printf("--> Region memoria: base=%x lenght=%x tipo=%d\n",base,lenght,tipo);  
            memoria_disponible += lenght;
            if (lenght > memoria_trabajo.lenght){
                memoria_trabajo.type = tipo;
                memoria_trabajo.lenght = lenght;
                memoria_trabajo.base = base;
            }
        }else if (tipo == LIMINE_MEMMAP_MEMORIA_KERNEL){
            memoria_kernel += lenght;
        }
    }

    printf("Memoria Total %x\nMemoria Dispoible %x\n",memoria_total,memoria_disponible);
    printf("Memoria usada por el kernel %x\n",memoria_kernel);
    printf("Region memoria de trabajo: base=%x lenght=%x tipo=%d\n",memoria_trabajo.base,memoria_trabajo.lenght,memoria_trabajo.type);
    iniciar_paginas();
}