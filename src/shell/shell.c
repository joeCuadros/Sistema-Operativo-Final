#include "shell.h"
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
// crear proceso
void crearProceso1(char * parte1){
    struct datos datos2;
    datos2.numero = 0;
    asignar_cadena(datos2.mensaje,parte1);
    agregarProceso(procesoImprimir100,&datos2,1);
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
    // crear proceso 
    }else if (comparar_subcadenas("crearProceso1[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        crearProceso1(parte2);
    // eliminar proceso 
    }else if (comparar_subcadenas("kill[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        uint32_t pid = convertir_a_entero(parte2);   
        if (pid == 0){ // no se puede eliminar kernel
            return;
        }
        eliminarProceso(pid); //eliminar proceso
    // ayuda
    }else if (comparar_cadenas("help",cadena)){
        printf("crearProceso1[<mensaje>]: Imprimir de 1 - 100\n");
        printf("kill[<pid>]: Eliminar proceso\n");
        printf("clear: Limpiar consola\n");
        printf("ps: Listar Proceso\n");
    }else{
        printf("?\n");
    }
}

// shell para ejecutar
int shell_comandos(struct datos* datos){
    printf("> %s ",datos->mensaje);
    habilitar_teclado(manejar_teclado);
    datos->numero = 0;
    return 1; //esperando
}