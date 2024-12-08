#include "bootservices.h"
#define MAX_STRING_SIZE 655536


//obtener longitud 
uint64_t strlen(const char* str){
    uint64_t len = 0;
    while (str[len] && len < MAX_STRING_SIZE)
        len++;
    return len;
}

void print(const char* str){
    void (*writer)(const char*, uint64_t) = escribir_terminal();
    writer(str, strlen(str));
}

void descansar(){
     while(1) {
        __asm__ ("hlt"); //dormir cpu
    } 
}

void _start() {
    print("Hola mundo desde DumpOS\n");
    print("A mimir ");
    descansar();
}