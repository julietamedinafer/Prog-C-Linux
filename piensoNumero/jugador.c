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
Solicita por teclado el nombre del jugador y lo graba en MEMORIA COMPARTIDA.
1) Espera el semáforo.
2) Verifica de MEMORIA COMPARTIDA:
a) Si numero_pensado = 0 y estado_acierto =0
i. Genera un número al azar de 1 al 99 (no repetitivos).
ii. Actualiza la variable numero_pensado con el número generado.
b) Si numero_pensado<> 0 y estada_acierto=1
i. Acertó, muestra por pantalla la cantidad de intentos.
3) Liberar el semáforo.
4) Si no hubo acierto, repite desde el punto 1.
5) Si hubo acierto, finaliza.*/

int main(int argc,char * argv[]){

    dato *memoria=NULL;
    int numero_elegido;
    int i= 0, cont= 1;

    /* aleatorio */
    inicializarAleatorio();
    
    /* memoria compartida */
    memoria= (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, IDKEY);

    /* semaforo */
    id_semaforo= creo_semaforo();

    /* limpieza */
    memset(memoria[i].nombre_ganador, 0x00, sizeof(memoria[i].nombre_ganador));

    printf("Ingrese su nombre: ");
    scanf("%s", memoria[i].nombre_ganador);

    while(1){
        espera_semaforo(id_semaforo);

        if(memoria[i].numero_pensado== 0 && memoria[i].estado_acierto== FALSE){
            printf("\nIntento numero %d\n", cont);
            numero_elegido= generarAleatorio(1,9);
            memoria[i].numero_pensado= numero_elegido;
            cont++;
        }

        if(memoria[i].numero_pensado!= 0 && memoria[i].estado_acierto== TRUE){
            printf("\nFelicidades %s, has adivinado el numero en %d intentos\n", memoria[i].nombre_ganador, cont);
            
            /* libero memoria compartida */
            libero_memoria((void*)memoria, id_memoria);
            levanta_semaforo(id_semaforo);
            
            return 0; 
        }
        
        levanta_semaforo(id_semaforo);
        usleep(ESPERAUS*1000);
    }
}
