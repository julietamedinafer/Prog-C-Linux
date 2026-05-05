#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "def.h"
#include "clave.h"
#include "global.h"
#include "cola.h"
#include "funciones.h" 

/* CONSIGNA:
Hilo Principal (main):
1- La cantidad de jugadores es siempre 6.
2- Borrar los mensajes anteriores para empezar de 0.
3- Elije, en forma aleatoria, un número del 1 al 6 y lo guarda en memoria local.
4- Le envía un mensaje a cada JUGADOR, EVT_INICIO. (MSG_JUGADOR+i).
5- Espera recibir mensajes de los JUGADORES.
EVT_DISPARO, NUMERO_ELEGIDO.
• Si NUMERO_ELEGIDO recibido es diferente al número del punto 3, contesta al JUGADOR
con EVT_SALVADO. (msg.int_rte). Sigue con el punto 6.
• Si NUMERO_ELEGIDO recibido es igual al número del punto 3, el “juego” finaliza; se envía un
mensaje EVT_FIN, NRO_JUGADOR (“eliminado”) a todos los JUGADORES
(MSG_JUGADOR+i). Sigue con el punto 7.
6- Repite desde el punto 5.
7- Finaliza*/

int main(int argc,char * argv[]){

	int num_jugador, posicion_bala;
	int fin= FALSE;
	int i;
	char mensaje_final[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */

	/* cola */
	id_cola= creo_id_cola();

	/* aleatorio */
	inicializar_aleatorio();
	posicion_bala= obtener_aleatorio(0, 5);
	printf("El revolver tiene la bala en la posicion: %d\n", posicion_bala);

	/* limpieza */
	borrar_mensaje(id_cola);  

	for(i=0;i<6;i++){
		enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_REVOLVER, EVT_INICIO, "");
	}

	while(fin== FALSE){
		recibir_mensaje(id_cola, MSG_REVOLVER, &msg);
		num_jugador= msg.remitente - MSG_JUGADOR; 

		switch(msg.evento){
			case EVT_INICIO:
				printf("\nIniciando juego...\n");
			break;
			case EVT_DISPARO:
				if(atoi(msg.contenidoMsg)== posicion_bala){
					printf("El jugador %d ha perdido.\n", num_jugador);
					sprintf(mensaje_final, "%d", num_jugador); /* enviar el numero del jugador eliminado */
					fin= TRUE;
					for(i=0;i<6;i++){
						enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_REVOLVER, EVT_FIN, mensaje_final);
					}
				}else{
					printf("El jugador %d se salvo.\n", num_jugador);
					enviar_mensaje(id_cola, MSG_JUGADOR+num_jugador, MSG_REVOLVER, EVT_SALVADO, "");
				}
			break;
		}
	}

	return 0;
}
