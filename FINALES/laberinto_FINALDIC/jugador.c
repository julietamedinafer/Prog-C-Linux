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
#include "archivos.h"
 

int main(int argc,char * argv[]){ 
	
	int i, j, cant_participantes;
	char linea[TAMANO_BUFFER];
	DatosParticipante *datos_th;

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();
	borrar_mensaje(id_cola);

	/* semaforo */
	id_semaforo= creo_semaforo();	
	
	/* cantidad de participantes */
	cant_participantes= 4;

	/* reservar memoria */
	id_hilo= (pthread_t *)malloc(sizeof(pthread_t)*cant_participantes);
	if(id_hilo== NULL){
		printf("Error: No se pudo reservar memoria para hilos\n");
		borrar_cola(id_cola);
		destruir_semaforo(id_semaforo);
		return -1;
	}
	
	datos_th= (DatosParticipante *)malloc(sizeof(DatosParticipante)*cant_participantes);
	if(datos_th== NULL){
		printf("Error: No se pudo reservar memoria para datos de participantes\n");
		free(id_hilo);
		borrar_cola(id_cola);
		destruir_semaforo(id_semaforo);
		return -1;
	}

	/* hilos */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutex, NULL);

	/* limpieza */
	memset(datos_th, 0x00, sizeof(DatosParticipante)*cant_participantes);
	memset(linea, 0x00, sizeof(linea));

	/* sincronizacion bidireccional */
	espera_semaforo(id_semaforo);
	abrirArchivo(ARCHIVO, "w");
	sprintf(linea, "%d", cant_participantes);
	escribirArchivo(linea);
	cerrarArchivo();
	
	abrirArchivo(FLAG_DOS, "w");
	escribirArchivo("listo");
	cerrarArchivo();
	levanta_semaforo(id_semaforo);

	/* esperar al otro proceso */
	while(1){
		espera_semaforo(id_semaforo);
		if(abrirArchivo(FLAG_UNO, "r")==0){
			cerrarArchivo();
			levanta_semaforo(id_semaforo);
			break;
		}
		levanta_semaforo(id_semaforo);
		usleep(100 * 1000);
	}

	/* inicializacion de datos de los hilos */
	for(i=0;i<cant_participantes;i++){
		datos_th[i].id_participante= i;
		datos_th[i].posicion= 0;
		datos_th[i].cont_avance= 0;
		datos_th[i].vivo= 1;
		datos_th[i].camino_elegido= 2;
		datos_th[i].meta_final= 150;

		if(pthread_create(&id_hilo[i], &attr, ThreadJugador, (void *)&datos_th[i])!= 0){
			printf("Error: No se pudo crear el thread %d\n", i);
			/* limpiar threads ya creados */
			for(j=0; j<i; j++){
				pthread_cancel(id_hilo[j]);
			}
			pthread_mutex_destroy(&mutex);
			pthread_attr_destroy(&attr);
			free(id_hilo);
			free(datos_th);
			borrar_cola(id_cola);
			destruir_semaforo(id_semaforo);
			return -1;
		}
	}

	/* sincronizacion */
	for(i=0;i<cant_participantes;i++){
		pthread_join(id_hilo[i], NULL);
	}

	/* resultados */
	printf("\nSimulacion del laberinto terminada.\n");

	/* limpieza de archivos de sincronizacion */
	remove(ARCHIVO);
	remove(FLAG_UNO);
	remove(FLAG_DOS);

	/* limpieza de recursos de threads */
	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attr);
	
	/* limpieza de semaforo */
	destruir_semaforo(id_semaforo);
	
	/* liberacion segura de memoria */
	if(id_hilo!= NULL){
		free(id_hilo);
		id_hilo= NULL;
	}
	if(datos_th!= NULL){
		free(datos_th);
		datos_th= NULL;
	}
	
	return 0;
}
