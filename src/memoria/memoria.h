#ifndef _MEMORIA_H
#define _MEMORIA_H
#define LIMINE_MEMMAP_UTILIZABLE             0
#define LIMINE_MEMMAP_RESERVADO              1
#define LIMINE_MEMMAP_ACPI_RECUPERABLE       2
#define LIMINE_MEMMAP_ACPI_NVS               3
#define LIMINE_MEMMAP_MEMORIA_INUTILIZABLE   4
#define LIMINE_MEMMAP_BOOTLOADER_RECUPERABLE 5
#define LIMINE_MEMMAP_MEMORIA_KERNEL         6
#define LIMINE_MEMMAP_SALIDA_GRAFICA         7
#include <stdint.h>

struct uefi_memory_region {
    uint64_t base;
    uint64_t lenght;
    uint64_t type;
};

struct permisos{
    uint8_t lectura : 1;  // 1 bit para lectura 
    uint8_t escritura : 1;  // 1 bit para escritura
    uint8_t ejecutable : 1;  // 1 bit para ejecutable
};

struct pagina_info{
    uint8_t estado : 1; // 0 libre, 1 = bloqueado
    struct permisos usuario;
    struct permisos demas;
    uint32_t pid; //propietario
};


void iniciar_memoria();
// solicitudes de paginas
void * solicitar_pagina (uint32_t pid);
void liberar_pagina(void* direccion,uint32_t pid);
void leer_pagina(void* destino, void* direccion_pagina,uint32_t pid);
void escribir_pagina(void* direccion_pagina, void* origen,uint32_t pid);
// permisos
void ver_permisos (void* direccion);
void set_usuario (void* direccion, struct permisos usuario_nuevo, uint32_t pid);
void set_demas (void* direccion, struct permisos demas_nuevo, uint32_t pid);
void copiar_bloque(const void* origen, void* destino, uint64_t n); // copiar bloque
void verMemoria();
#endif