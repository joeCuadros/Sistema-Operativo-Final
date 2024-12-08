#include "memoria.h"
#include "../bootloader/bootservices.h" 
#include "../printf/printf.h" //para escribir en pantalla

struct uefi_memory_region memoria_trabajo = {
    .type = LIMINE_MEMMAP_MEMORIA_INUTILIZABLE,
    .lenght = 0,
    .base = 0
};

void iniciar_memoria(){
    uint64_t entradas = obtener_memoria_map_entrada();
    printf("Se encontro %x regiones de memoria\n",entradas);
    uint64_t memoria_total = 0;
    uint64_t memoria_disponible = 0;

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

    printf("Memoria Total %x\nMemoria Dispoible %x",memoria_total,memoria_disponible);
    printf("Region memoria de trabajo: base=%x lenght=%x tipo=%d\n",memoria_trabajo.base,memoria_trabajo.lenght,memoria_trabajo.type);
}