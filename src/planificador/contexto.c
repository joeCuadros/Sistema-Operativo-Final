#include "contexto.h"

// Guardar el contexto actual en la estructura contexto_buf
void guardar_contexto(contexto_buf* contexto) {
    __asm__ volatile (
        "mov %%eax, %0\n"
        "mov %%ebx, %1\n"
        "mov %%ecx, %2\n"
        "mov %%edx, %3\n"
        "mov %%esi, %4\n"
        "mov %%edi, %5\n"
        "mov %%ebp, %6\n"
        "mov %%esp, %7\n"
        // Aquí almacenamos la dirección de retorno directamente desde la pila
        "mov (%%esp), %%eax\n"  // La dirección de retorno está en la cima de la pila
        "mov %%eax, %8\n"       // Guardamos la dirección de retorno en contexto->eip
        : "=m" (contexto->eax), "=m" (contexto->ebx), "=m" (contexto->ecx),
          "=m" (contexto->edx), "=m" (contexto->esi), "=m" (contexto->edi),
          "=m" (contexto->ebp), "=m" (contexto->esp), "=m" (contexto->eip)
        :
        : "eax"
    );
}

// Restaurar el contexto guardado en la estructura contexto_buf
void restaurar_contexto(contexto_buf* contexto) {
    __asm__ volatile (
        "mov %0, %%eax\n"
        "mov %1, %%ebx\n"
        "mov %2, %%ecx\n"
        "mov %3, %%edx\n"
        "mov %4, %%esi\n"
        "mov %5, %%edi\n"
        "mov %6, %%ebp\n"
        "mov %7, %%esp\n"
        // Saltamos a la dirección guardada en contexto->eip
        "jmp *%8\n"            // Esto hará un salto a la dirección almacenada en contexto->eip
        :
        : "m" (contexto->eax), "m" (contexto->ebx), "m" (contexto->ecx),
          "m" (contexto->edx), "m" (contexto->esi), "m" (contexto->edi),
          "m" (contexto->ebp), "m" (contexto->esp), "m" (contexto->eip)
    );
}
