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

/*CONSIGNA:
El hilo principal (main):
• La cantidad de jugadores es siempre 6.
• Genera un vector de 6 posiciones inicializado en 0, llamado “vector_tambor”, [0][0][0][0][0][0].
Este vector simula la posición de cada bala.
• Lanza tantos HiloJUGADOR como JUGADORES haya, y les pasa un puntero con la dirección
del “vector_tambor”.
• Se queda esperando que terminen los hilos.
• Muestra el número del JUGADOR “eliminado” por pantalla (recibido de los hilos).
*/

int main(int argc,char * argv[]){
	
	int i;
	int cant_jugadores= 6;
	int *vector_tambor;
	DatosJugador *datos_th;

	vector_tambor= (int *)malloc(sizeof(int)*cant_jugadores);

	/* hilos  y atributos */
	id_hilo= (pthread_t *)malloc(sizeof(pthread_t)*cant_jugadores);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutex, NULL);

	/* datos de los hilos */
	datos_th= (DatosJugador *)malloc(sizeof(DatosJugador)*cant_jugadores);

	/* cola */
	id_cola= creo_id_cola();

	/* aleatorio */
	inicializar_aleatorio();

	/* limpieza */
	memset(vector_tambor, 0x00, sizeof(int)*cant_jugadores);

	/* inicializacion de datos de los hilos */
	for(i=0;i<cant_jugadores;i++){
		datos_th[i].num_jugador= i;
		datos_th[i].id_cola_mensaje= id_cola;
		datos_th[i].vector_tambor= vector_tambor;
		datos_th[i].fue_eliminado= FALSE;

		pthread_create(&id_hilo[i], &attr, ThreadJugador, (void *)&datos_th[i]);
	}

	for(i=0;i<cant_jugadores;i++){
		pthread_join(id_hilo[i], NULL);
	}
	
	/* buscar jugador eliminado */
	for(i=0;i<cant_jugadores;i++){
		if(datos_th[i].fue_eliminado == TRUE) {
			printf("El jugador eliminado es: %d\n", datos_th[i].num_jugador);
			break;
		}
	}

	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attr);
	free(vector_tambor);
	free(id_hilo);
	free(datos_th); 

	return 0;
}
