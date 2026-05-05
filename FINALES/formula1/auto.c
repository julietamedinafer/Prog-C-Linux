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
	
	int i, cant_participantes;
	char linea[TAMANO_BUFFER];
	DatosParticipante *datos_th;

	/*if(argc!= 2){
		printf("\nParametro inválido.\nDebe ingresar cantidad de autos.\nEjemplo: ./auto 2\n");
		return 0;
	}else{
        cant_participantes=atoi(argv[1]);
    }*/

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();
	borrar_mensaje(id_cola);

	/* semaforo */
	id_semaforo= creo_semaforo();	
	
	/* cantidad de participantes manual */
	cant_participantes= 10;

	/* reservar memoria */
	id_hilo= (pthread_t *)malloc(sizeof(pthread_t)*cant_participantes);
	datos_th= (DatosParticipante *)malloc(sizeof(DatosParticipante)*cant_participantes);

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
		datos_th[i].id_auto= i;
		datos_th[i].posicion= 0;
		datos_th[i].puntos= 0;
		datos_th[i].cont_avance= 0;

		pthread_create(&id_hilo[i], &attr, ThreadJugador, (void *)&datos_th[i]);
	}

	/* sincronizacion */
	for(i=0;i<cant_participantes;i++){
		pthread_join(id_hilo[i], NULL);
	}

	/* resultados */
	/*for(i=0;i<cant_participantes;i++){
		if(datos_th[i].ganador== TRUE){
			printf("El ganador es el auto %d\n", datos_th[i].id_auto+1);
		}
	}*/

	/* limpieza de archivos de sincronizacion */
	remove(ARCHIVO);
	remove(FLAG_UNO);
	remove(FLAG_DOS);

	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attr);
	free(id_hilo);
	free(datos_th);
	return 0;
}
