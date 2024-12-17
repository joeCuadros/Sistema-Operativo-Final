#include "shell.h"
#include "../memoria/memoria.h"
#define MAX_CADENA 256  // Tamaño del buffer
extern volatile struct proceso *procesoActual;
// recepciona teclado
void manejar_teclado(char caracter) {
    static char cadena[MAX_CADENA] = "";  
    static int index = 0;  

    if (index < MAX_CADENA - 1) {  // Verifica que no exceda el tamaño del buffer
        if (caracter == '\n') {
            printf("\n", cadena);  
            cli_asm(); //desactivar interrupciones
            comandoBuscar(cadena);
            index = 0; 
            llenar_memoria(cadena,0,MAX_CADENA);
            cadena[0] = '\0';
            printf("> ");
            sti_asm(); //activar interrupciones
        } else if (caracter == '\b') {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
        } else {
            cadena[index++] = caracter;
            printf("%c", caracter);
        }
    }
}
// crear proceso implementar despues
void crearProceso0(){
    struct datos datos2;
    datos2.numero = 0;
    agregarProceso(procesoInfinto,&datos2,1);
}
//crear proceso del 1 al 100
void crearProceso1(char * parte){
    struct datos datos2;
    datos2.numero = 0;
    asignar_cadena(datos2.mensaje,parte);
    agregarProceso(procesoImprimir100,&datos2,1);
}

void crearProceso2(char * parte){
    struct datos datos2;
    datos2.numero = 0;
    asignar_cadena(datos2.mensaje,parte);
    agregarProceso(procesoImprimir_0,&datos2,0);
}

void accionarVariosProcesos(){
    struct datos datos1;
    datos1.numero = 20;
    asignar_cadena(datos1.mensaje,"A");
    agregarProceso(procesoImprimir_0,&datos1,1);
    struct datos datos2;
    datos2.numero = 13;
    asignar_cadena(datos2.mensaje,"B");
    agregarProceso(procesoImprimir_0,&datos2,1);
    struct datos datos3;
    datos3.numero = 22;
    asignar_cadena(datos3.mensaje,"C");
    agregarProceso(procesoImprimir_0,&datos3,1);
    struct datos datos4;
    datos4.numero = 8;
    asignar_cadena(datos4.mensaje,"D");
    agregarProceso(procesoImprimir_0,&datos4,1);
}   

// buscar comando 
void comandoBuscar(char * cadena){
    char* parte1;
    char* parte2;
    if (comparar_cadenas("clear",cadena)){
        limpiar_pantalla();
    // listar
    }else if (comparar_cadenas("ps",cadena)){
        listaProcesos();
    // listar
    }else if (comparar_cadenas("free",cadena)){
        verMemoria();
    // listar
    }else if (comparar_cadenas("run1",cadena)){
        accionarVariosProcesos();
    // crear proceso
    }else if (comparar_subcadenas("crearProceso2[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        crearProceso2(parte2);
    // crear proceso 
    }else if (comparar_subcadenas("crearProceso1[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        crearProceso1(parte2);
    // crear proceso 2 plano
    }else if (comparar_cadenas("crearProceso0",cadena)){
        crearProceso0();
    // eliminar proceso 
    }else if (comparar_subcadenas("kill[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        uint32_t pid = convertir_a_entero(parte2);   
        if (pid == 0){ // no se puede eliminar kernel
            printf("No existe el pid o restringido\n");
            return;
        }
        eliminarProceso(pid); //eliminar proceso
    // ayuda
    }else if (comparar_cadenas("help",cadena)){
        printf("run1: Crear 4 procesos\n");
        printf("crearProceso0: Crear proceso fantasma\n");
        printf("crearProceso1[<mensaje>]: Imprime Mensaje de 1 - 100\n");
        printf("crearProceso2[<mensaje>]: Imprime Mensaje de 1 - 100 prioridad 0\n");
        printf("free: estado de memoria \n");
        printf("kill[<pid>]: Eliminar proceso\n");
        printf("clear: Limpiar consola\n");
        printf("ps: Listar Proceso\n");
    }else{
        printf("?\n");
    }
}

// shell para ejecutar
int shell_comandos(struct datos* datos){
    printf("> ");
    habilitar_teclado(manejar_teclado);
    datos->numero = 0;
    return 1; //esperando
}