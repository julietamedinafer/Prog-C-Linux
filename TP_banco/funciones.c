#include "funciones.h"
#include "stdlib.h"
int INdevolverNumeroAleatorio(int min, int max){
    int Numero;
    Numero = rand () % (max-min+1) + min;
    return Numero;
}
