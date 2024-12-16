#include "proceso.h"
#include "../printf/printf.h"
#define NUEVO       0
#define LISTO       1
#define EJECUTANDO  2
#define ESPERANDO   3
#define TERMINADO   4
/*simulando un proceso*/
// proceso que se pueden usar como bash
int procesoA(struct datos* datos){
    datos->numero = 0;
    while(datos->numero < 100){
        printf("Solo proceso %s %d\n",datos->mensaje,datos->numero);
        datos->numero++;
    }
    return 0;
}
