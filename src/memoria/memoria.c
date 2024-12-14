#include "memoria.h"
#include "../bootloader/bootservices.h" 
#include "../printf/printf.h" //para escribir en pantalla
#include "../util/string.h"
#include "../util/error.h" 
#define MAXIMO_MARCO 4096

#define DIRECCION_A_marco(x) ((uint64_t)(x) >> 12) //desplazamiento de bits 2^12 = 4096

struct uefi_memory_region memoria_trabajo = {
    .type = LIMINE_MEMMAP_MEMORIA_INUTILIZABLE,
    .lenght = 0,
    .base = 0
};

uint64_t ultimo_marco;
uint64_t total_marcos;
struct marco_info *region_memoria;

void bloquear_marco(uint64_t marco){
    region_memoria[marco].estado = 1;
    ultimo_marco = marco;
}

void desbloquear_marco(uint64_t marco){
    region_memoria[marco].estado = 0;
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
        kernel_panic("Error al intentar liberar marco fuera de la region de memoria");
    }
    uint64_t direccion_normalizada = (direccion_uint64 - memoria_trabajo.base);
    return DIRECCION_A_marco(direccion_normalizada);
}



// eliminacion logico del recurso 
void liberar_marco(void* direccion, uint32_t pid){
    uint64_t direccion_uint64 = (uint64_t) direccion;
    uint64_t indice_marco = normalizar(direccion_uint64);
    if (region_memoria[indice_marco].pid != pid){
        error_kernel(pid,"memoria","No se puede liberar marco que no tienes permiso");
        return ;
    }

    if (region_memoria[indice_marco].estado == 0){
        error_kernel(pid,"memoria","Se intenta liberar marco que no esta bloqueada");
        return ;
    }
    desbloquear_marco(indice_marco);
    if (region_memoria[indice_marco].estado == 1){
        kernel_panic("Fallo al liberar marco");
    }
}

// solicitar marco nuevo (NO OLVIDAR)
void * solicitar_marco (uint32_t pid){
    uint64_t primero = ultimo_marco-1;

    while(region_memoria[ultimo_marco].estado){ // 1 = true, 0 = false
        if(ultimo_marco >= total_marcos){
            ultimo_marco = 0;
        }  
        if (ultimo_marco == primero){
            kernel_panic("No hay memoria disponible ");//implementar algoritmo de recolocacion (NO OLVIDAR)
        }
        ultimo_marco++;
    }

    bloquear_marco(ultimo_marco);
    region_memoria[ultimo_marco].pid = pid;
    region_memoria[ultimo_marco].demas.ejecutable = 0;
    region_memoria[ultimo_marco].demas.escritura = 0;
    region_memoria[ultimo_marco].demas.lectura = 0;
    region_memoria[ultimo_marco].usuario.ejecutable = 0;
    region_memoria[ultimo_marco].usuario.escritura = 1;
    region_memoria[ultimo_marco].usuario.lectura = 1;

    if (region_memoria[ultimo_marco].estado == 0){
        kernel_panic("Fallo al solicitar marco");
    }

    return (void*) ((uint8_t*) memoria_trabajo.base + (ultimo_marco * 0x1000));
}
// leer marco
void leer_marco(void* destino, void* direccion_marco,uint32_t pid){
    uint64_t direccion_uint64 = (uint64_t) direccion_marco;
    uint64_t indice_marco = normalizar(direccion_uint64);
    if (region_memoria[indice_marco].pid == pid && region_memoria[indice_marco].usuario.lectura) {
        copiar_bloque(direccion_marco, destino, MAXIMO_MARCO); // Copiar de direccion_marco a destino
    } else if(region_memoria[indice_marco].demas.lectura){
        copiar_bloque(direccion_marco, destino, MAXIMO_MARCO); 
    } else {
        error_kernel(pid, "memoria", "No tiene permiso escritura");
    }
    
}   
// escribir marco
void escribir_marco(void* direccion_marco, void* origen,uint32_t pid){
    uint64_t direccion_uint64 = (uint64_t) direccion_marco;
    uint64_t indice_marco = normalizar(direccion_uint64);
    if (region_memoria[indice_marco].pid == pid && region_memoria[indice_marco].usuario.escritura) {
        copiar_bloque(origen, direccion_marco, MAXIMO_MARCO); // Copiar de origen a direccion_marco
    } else if(region_memoria[indice_marco].demas.escritura){
        copiar_bloque(origen, direccion_marco, MAXIMO_MARCO);
    } else {
        error_kernel(pid, "memoria", "No tiene permiso escritura");
    }
    
}  

void ver_permisos (void* direccion){
    uint64_t direccion_uint64 = (uint64_t) direccion;
    uint64_t indice_marco = normalizar(direccion_uint64);
    // comprobar si usuario
    printf("Propietario (PID): %llx ", region_memoria[indice_marco].pid);
    (region_memoria[indice_marco].usuario.lectura) ? printf("R") : printf("-");
    (region_memoria[indice_marco].usuario.escritura) ? printf("W") : printf("-");
    (region_memoria[indice_marco].usuario.ejecutable) ? printf("X") : printf("-");
    printf("\t");
    (region_memoria[indice_marco].demas.lectura) ? printf("R") : printf("-");
    (region_memoria[indice_marco].demas.escritura) ? printf("W") : printf("-");
    (region_memoria[indice_marco].demas.ejecutable) ? printf("X") : printf("-");
    printf("\n");
}

void set_usuario (void* direccion, struct permisos usuario_nuevo,uint32_t pid){
    uint64_t direccion_uint64 = (uint64_t) direccion;
    uint64_t indice_marco = normalizar(direccion_uint64);
    if (region_memoria[indice_marco].pid == pid){
        region_memoria[indice_marco].usuario = usuario_nuevo;
    }else{
        error_kernel(pid, "memoria permisos", "No puede alterar permiso");
    }
}
void set_demas (void* direccion, struct permisos demas_nuevo,uint32_t pid){
    uint64_t direccion_uint64 = (uint64_t) direccion;
    uint64_t indice_marco = normalizar(direccion_uint64);
    if (region_memoria[indice_marco].pid == pid){
        region_memoria[indice_marco].demas = demas_nuevo;
    }else{
        error_kernel(pid, "memoria permisos", "No puede alterar permiso");
    }
}

// iniciar marcos de memoria
void iniciar_marcos(){
    total_marcos = memoria_trabajo.lenght / 0x1000; //4096 en hexadecimal
    printf("Total de marcos %x\n",total_marcos);
    region_memoria = (struct marco_info *)memoria_trabajo.base;
    llenar_memoria((void*)region_memoria, 0, total_marcos); 
    for (uint64_t i = 0; i < (total_marcos / 0x1000); i++ ){
        bloquear_marco(i);
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
    iniciar_marcos();
}