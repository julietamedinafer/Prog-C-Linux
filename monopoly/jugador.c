#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "def.h"
#include "clave.h"
#include "global.h"
#include "cola.h" 
#include "funciones.h"
#include "semaforo.h"
#include "memoria.h"
#include "thread.h"


int main(int argc,char * argv[]){ 
	
	int i, cant_jugadores;
	DatosJugador *datos_th;
	Sincronizacion *memoria;

	if(argc!= 2){
		printf("\nParametro inválido.\nDebe ingresar cantidad de jugadores.\nEjemplo: ./jugador 2\n");
		return 0;
	}else{
        cant_jugadores=atoi(argv[1]);
    }

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();

	/* sincronizacion entre procesos */
	memoria= (Sincronizacion*)creo_memoria(sizeof(Sincronizacion), &id_memoria, IDKEY);

	/* semaforo */
	id_semaforo= creo_semaforo();

	/* reservar memoria */
	id_hilo= (pthread_t *)malloc(sizeof(pthread_t)*cant_jugadores);
	datos_th= (DatosJugador *)malloc(sizeof(DatosJugador)*cant_jugadores);

	/* hilos */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutex, NULL);

	/* limpieza */
	memset(datos_th, 0x00, sizeof(DatosJugador)*cant_jugadores);

	/* sincronizacion entre procesos parte 2*/
	espera_semaforo(id_semaforo);
    memoria->proceso_2_preparado= 1;
    memoria->cant_jugadores= cant_jugadores;
    levanta_semaforo(id_semaforo);
    
    while(1){
        espera_semaforo(id_semaforo);
        if(memoria->proceso_1_preparado== 1){
            levanta_semaforo(id_semaforo);
            break;
        }
        levanta_semaforo(id_semaforo);
        usleep(100 * 1000);
    }

	/* inicializacion de datos de los hilos */
	for(i=0;i<cant_jugadores;i++){
		datos_th[i].id_jugador= i;
		datos_th[i].posicion= 0;
		datos_th[i].cant_pasos= 0;
		datos_th[i].saldo= 1000;
		datos_th[i].ganador= FALSE;

		pthread_create(&id_hilo[i], &attr, ThreadJugador, (void *)&datos_th[i]);
	}

	/* sincronizacion */
	for(i=0;i<cant_jugadores;i++){
		pthread_join(id_hilo[i], NULL);
	}

	/* resultados */
	for(i=0;i<cant_jugadores;i++){
		if(datos_th[i].ganador== TRUE){
			printf("El ganador es el jugador %d\n", datos_th[i].id_jugador+1);
		}
	}

	libero_memoria((char *)memoria, id_memoria);
	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attr);
	free(id_hilo);
	free(datos_th); 

	return 0;
}
