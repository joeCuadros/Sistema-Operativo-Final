#include "proceso.h"
#include "../printf/printf.h"
extern volatile struct proceso *procesoActual;
#define NUEVO       0
#define LISTO       1
#define EJECUTANDO  2
#define ESPERANDO   3
#define TERMINADO   4
/*simulando un proceso*/
// proceso que imprime hasta el 100
int procesoImprimir100(struct datos* datos){
    datos->numero++;
    printf("(P %s %d) -> PID (%d)",datos->mensaje,datos->numero,procesoActual->pid);
    if (datos->numero < 100) {return -1;}
    return 0;
}
// proceso fantasma
int procesoInfinto(struct datos* datos){
    datos->numero++;
    return -1;
}