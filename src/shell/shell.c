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
// crear proceso implementar despues
void crearProceso0(){
    struct datos datos2;
    datos2.numero = 0;
    agregarProceso(procesoInfinto,&datos2,1);
}
void crearProceso1(char * parte){
    struct datos datos2;
    datos2.numero = 0;
    asignar_cadena(datos2.mensaje,parte);
    agregarProceso(procesoImprimir100,&datos2,1);
}

void crearProceso2(){
    struct datos datos2;
    datos2.numero = 0;
    agregarProceso(procesoImprimir_0,&datos2,1);
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
    // ver memoria RAM por regiones 
    }else if (comparar_subcadenas("verMemoriaRegion[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        uint32_t cantidad = convertir_a_entero(parte2);   
        if (cantidad == 0){ // es erroneo y no valido
            printf("Cantidad Invalida\n");
            return;
        }
        estado_memoria_RAM(cantidad);
    // ver memoria RAM por pid 
    }else if (comparar_subcadenas("verMemoria[", cadena)){
        separar_cadena(cadena, &parte1, &parte2);
        uint32_t pid = convertir_a_entero(parte2);  // si no da que sea del sistema 
        if (pid== 0){ // es erroneo y no valido
            printf("No existe el pid o restringido\n");
        estado_memoria_RAM_pid(pid);
        }
    // ver paginas de RAM
    }else if (comparar_cadenas("verRam", cadena)){
        paginasUsadas();
    // ayuda
    }else if (comparar_cadenas("help",cadena)){
        printf("crearProceso0: Crear proceso fantasma\n");
        printf("crearProceso1[<mensaje>]: Imprime Mensaje de 1 - 100\n");
        //printf("crearProceso2[<mensaje>,<cantidad>]: Imprimer mensaje desde x a 0\n");
        printf("verMemoriaRegion[<cantidad>]: Ver la cantidad de paginas \n");
        printf("verMemoria[<pid>]: Ver la cantidad de paginas del pid \n");
        printf("verRam: Ver la cantidad de paginas de RAM disponibles \n");
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