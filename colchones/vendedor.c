#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "semaforo.h"
#include "funciones.h"
#include "def.h"
#include "global.h"
#include "memoria.h"

/* CONSIGNA:
1) Espera SEMÁFORO.
2) Solicita por TECLADO (OBLIGATORIO) un código y cantidad (1 a 5).
3) Resta esta cantidad al código seleccionado. NUNCA DEJA EL STOCK MENOR QUE 0.
4) Libera SEMÁFORO.
5) Vuelve al punto 1.*/

int main(int argc,char * argv[]){

    dato *memoria=NULL;
    int i= 0;
    int cod, cant;

    /* aleatorio */
    inicializarAleatorio();
    
    /* memoria compartida */
    memoria= (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, IDKEY);

    /* semaforo */
    id_semaforo= creo_semaforo();

    while(1){
        espera_semaforo(id_semaforo);

        printf("\nIngrese un codigo (1 a 5) o 0 para salir: ");
        scanf("%d", &cod);
        while(cod< 0 || cod> 5){
            printf("\nError, ingrese un codigo valido (1 a 5) o 0 para salir: ");
            scanf("%d", &cod);
        }

        if(cod== 0){
            memoria[0].codigo= 0;
            levanta_semaforo(id_semaforo);
            usleep(ESPERAUS*1000);
            /* libero memoria compartida */
            libero_memoria((void*)memoria, id_memoria);
            return 0;
        }
        
        printf("\nIngrese una cantidad: ");
        scanf("%d", &cant);
        while(cant< 1 || cant> 10){
            printf("\nError, ingrese una cantidad valida (1 a 10): ");
            scanf("%d", &cant);
        }

        for(i= 0; i< CANTIDAD; i++){
            if(memoria[i].codigo== cod){
                if((memoria[i].cantidad - cant) < 0){
                    printf("\nNo hay stock suficiente\n");
                }else{
                    memoria[i].cantidad= memoria[i].cantidad - cant;
                    printf("\nCompra realizada\n");
                }
            }
        }

        levanta_semaforo(id_semaforo); 
        usleep(ESPERAUS*1000);
    }
}
