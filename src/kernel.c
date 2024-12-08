#include "bootloader/bootservices.h"
#include "printf/printf.h" 
#define MAX_STRING_SIZE 655536


//obtener longitud 
uint64_t strlen(const char* str){
    uint64_t len = 0;
    while (str[len] && len < MAX_STRING_SIZE)
        len++;
    return len;
}
/* 
void print(const char* str){
    void (*writer)(const char*, uint64_t) = escribir_terminal();
    writer(str, strlen(str));
}*/

void descansar(){
     while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}

void _start() {
    printf("Hola mundo desde DumpOS\n");
    printf("Disposiciones tecnicas: bootloader=%s, version=%s\n",obtener_bootloader_nombre(),obtener_bootloader_version());
    descansar();
}