// NO usar 
#ifndef _CONTEXTO_H  
#define _CONTEXTO_H 
#include <stdint.h>

typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
} contexto_buf;

void guardar_contexto(contexto_buf* contexto);
void restaurar_contexto(contexto_buf* contexto);

#endif
