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
1) “Piensa un número”, genera en forma aleatoria un número del 1 al 9 y lo guarda
en una variable local intlocal_pienso_un_numero;
2) Inicializa la MEMORIA COMPARTIDA y el SEMÁFORO.
3) Espera el semáforo.
a) Si numero_pensado<> 0 y estado_acierto =0
i. Si numero_pensado = local_pienso_un_numero.
Actualiza estado_acierto = 1
ii. Si numero_pensado<>local_pienso_un_numero.
Actualiza numero_pensado = 0 y estado_acierto =0
4) Libero el SEMÁFORO.
5) Si hubo acierto, muestra por pantalla el nombre del jugador (sacarlo de MEMORIA
COMPARTIDA) ganador y finaliza.
6) Si no hubo acierto, regresa al punto 3.*/

int main(int argc,char * argv[]){

    dato *memoria= NULL;
    int numero_local;
    int i= 0;

    /* aleatorio */
    inicializarAleatorio();
    numero_local= generarAleatorio(1,9);
    printf("Numero a adivinar: %d\n", numero_local);

    /* memoria compartida */
    memoria= (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, IDKEY);

    /* semaforo */
    id_semaforo= creo_semaforo();
    inicia_semaforo(id_semaforo,VERDE);
    

    while(1){
        espera_semaforo(id_semaforo);
        if(memoria[i].numero_pensado!= 0 && memoria[i].estado_acierto== FALSE){
            if(memoria[i].numero_pensado== numero_local){
                memoria[i].estado_acierto= TRUE;
                printf("\nEl jugador %s ha adivinado\n", memoria[i].nombre_ganador);

                levanta_semaforo(id_semaforo);
                usleep(ESPERAUS*1000);

                /* libero memoria compartida */
                libero_memoria((char*)memoria, id_memoria);
                
                return 0;

            }else{
                memoria[i].numero_pensado= 0;
                memoria[i].estado_acierto= FALSE;
            }
        }

        levanta_semaforo(id_semaforo);
        usleep(ESPERAUS*1000);
    }
}
