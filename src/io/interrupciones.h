#ifndef _INTERRUPCIONES_H
#define _INTERRUPCIONES_H

struct marco_interrupciones;

__attribute__((interrupt)) void Excepcion_General(struct marco_interrupciones* marco);
void iniciar_interrupciones();

#endif