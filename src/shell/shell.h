#ifndef _SHELL_H
#define _SHELL_H
#include <stdint.h>
#include "../entrada/teclado.h"
#include "../planificador/proceso.h"
#include "../planificador/planificador.h"
#include "../printf/printf.h"
#include "../util/string.h"
#include "../io/bajo_nivel.h"
#include "../memoria/memoria.h"

void manejar_teclado(char caracter);
int shell_comandos(struct datos* datos);
void comandoBuscar(char * cadena);

#endif