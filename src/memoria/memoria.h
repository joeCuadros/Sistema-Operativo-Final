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

void iniciar_memoria();
void * solicitar_pagina ();
void liberar_pagina(void* direccion);

#endif