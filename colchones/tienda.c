#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "semaforo.h"
#include "funciones.h"
#include "def.h"  
#include "global.h"
#include "memoria.h"

/*CONSIGNA:
1) Carga la descripción de cada código por programa. La cantidad inicial es para todos
10;
2) 1 PIERO, 2 SUAVESTAR, 3 CANNON, 4 SIMMONS, 5 BELMO
3) Espera SEMÁFORO
4) Revisa la cantidad de cada COLCHÓN; si es menor que 10, suma 1 al stock (SIEMPRE).
NUNCA SE PASA DE 10.
5) Libera SEMÁFORO.
6) Espera 500ms.
7) Repite desde el punto 2.*/

int main(int argc,char * argv[]){

    dato *memoria= NULL;
    int i= 0;

    /* aleatorio */
    inicializarAleatorio();

    /* memoria compartida */
    memoria= (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, IDKEY);

    /* semaforo */
    id_semaforo= creo_semaforo();
    inicia_semaforo(id_semaforo,VERDE);

    /* limpieza */
    memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));

    /* carga inicial */
    for(i= 0; i< CANTIDAD; i++){
        memoria[i].codigo= i+1;
        memoria[i].cantidad= 10;
        if(memoria[i].codigo== 1){
            memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));
            strcpy(memoria[i].nombre, "PIERO");
        }
        if(memoria[i].codigo== 2){
            memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));
            strcpy(memoria[i].nombre, "SUAVESTAR");
        }
        if(memoria[i].codigo== 3){
            memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));
            strcpy(memoria[i].nombre, "CANNON");
        }
        if(memoria[i].codigo== 4){
            memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));
            strcpy(memoria[i].nombre, "SIMMONS");
        }
        if(memoria[i].codigo== 5){
            memset(memoria[i].nombre, 0x00, sizeof(memoria[i].nombre));
            strcpy(memoria[i].nombre, "BELMO");
        }
    }

    while(1){
        espera_semaforo(id_semaforo);

        if(memoria[0].codigo== 0){
            /* libero memoria compartida */
            libero_memoria((void*)memoria, id_memoria);
            return 0;
        }

         /* limpio pantalla */
        printf("\033[H\033[J");

        for(i= 0; i< CANTIDAD; i++){         
            printf("\n%d %s cantidad: %d\n", memoria[i].codigo, memoria[i].nombre, memoria[i].cantidad);
            if(memoria[i].cantidad< 10){
                memoria[i].cantidad++;
            }
        }

        levanta_semaforo(id_semaforo);
        usleep(ESPERAUS*1000);
    }         
}
