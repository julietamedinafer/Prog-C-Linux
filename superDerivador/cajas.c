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
#include "thread.h"
#include "archivos.h"
#include "semaforo.h"


int main(int argc,char * argv[]){
	
	int i;
	DatosSuper *datos_th;

	/* aleatorio */

	/* semaforo  */

	/* cola */
	id_cola= creo_id_cola();

	/* reservar memoria */
	id_hilo= (pthread_t *)malloc(sizeof(pthread_t)*CANTIDAD);
	datos_th= (DatosSuper *)malloc(sizeof(DatosSuper)*CANTIDAD);

	/* hilos */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutex, NULL);

	/* limpieza */
	memset(datos_th, 0x00, sizeof(DatosSuper)*CANTIDAD);

	/* inicializacion de datos de los hilos */
	for(i=0;i<CANTIDAD;i++){
		datos_th[i].id_caja= i;
		datos_th[i].total= 0;

		pthread_create(&id_hilo[i], &attr, ThreadJugador, (void *)&datos_th[i]);
	}

	/* sincronizacion */
	for(i=0;i<CANTIDAD;i++){
		pthread_join(id_hilo[i], NULL);
	}

	/* resultados */
	/* for(i=0;i<CANTIDAD;i++){
		if(datos_th[i].ganador== TRUE) {
			printf("El Super ganador es: %s\n", datos_th[i].nombre_Super);
			break; 
		}
	} */

	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attr);
	free(id_hilo);
	free(datos_th); 

	return 0;
}
