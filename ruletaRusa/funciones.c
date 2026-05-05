#include <stdlib.h>
#include <time.h>
#include "funciones.h"

void inicializar_aleatorio(void){
	srand(time(NULL));
}

int obtener_aleatorio(int desde, int hasta){
	return (rand()%(hasta-desde+1))+desde;
}
