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


int main(int argc,char * argv[]){

	int num_animal, cant_pasos, cont_ronda, ganador;
	char nombre_animal[TAMANO_BUFFER];
	int ganadores_ronda[3] = {FALSE, FALSE, FALSE};
	int hay_ganadores = FALSE;
	int i, respuestas_recibidas = 0;
	int fin= FALSE;
	char msg_enviar[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */

	/* cola */
	id_cola= creo_id_cola();

	/* aleatorio */

	/* limpieza */
	borrar_mensaje(id_cola); 
	memset(&msg, 0x00, sizeof(mensaje));
	memset(msg_enviar, 0x00, sizeof(msg_enviar));

	printf("\nBIENVENIDO A LA CARRERA\n\n");
	
	/* señal a los participantes para iniciar */
	for(i= 0;i< CANTIDAD;i++){
		enviar_mensaje(id_cola, MSG_ANIMAL+i, MSG_PISTA, EVT_INICIO, "");
	}

	while(fin== FALSE){
		recibir_mensaje(id_cola, MSG_PISTA, &msg);
		num_animal = msg.remitente - MSG_ANIMAL;

		switch(msg.evento){
			case EVT_INICIO:
				printf("Animal %d en posicion\n", num_animal); 
				respuestas_recibidas++;
				
				if(respuestas_recibidas == CANTIDAD){
					printf("\nInicia la carrera\n");
					for(i=0; i<CANTIDAD; i++){
						enviar_mensaje(id_cola, MSG_ANIMAL+i, MSG_PISTA, EVT_CORRO, "");
					}
					respuestas_recibidas = 0; 
				}
			break;
			case EVT_CORRO:
				sscanf(msg.contenidoMsg, "%s %d %d %d", nombre_animal, &cant_pasos, &cont_ronda, &ganador);
				printf("El %s avanzo %d pasos en la ronda %d\n", nombre_animal, cant_pasos, cont_ronda);
				respuestas_recibidas++;
				
				ganadores_ronda[num_animal]= ganador;
				
				if(ganador == TRUE){
					printf("\nGanador: %s con %d pasos en %d rondas\n", nombre_animal, cant_pasos, cont_ronda);
				}
				
				if(respuestas_recibidas== CANTIDAD){
					/* Verificar si hay al menos un ganador en esta ronda */
					for(i = 0; i < CANTIDAD; i++){
						if(ganadores_ronda[i] == TRUE){
							hay_ganadores = TRUE;
							break;
						}
					}
					
					if(hay_ganadores == TRUE){
						printf("\n--- Juego terminado ---\n");
						for(i=0; i<CANTIDAD; i++){
							enviar_mensaje(id_cola, MSG_ANIMAL+i, MSG_PISTA, EVT_FIN, "");
						}
						fin = TRUE;
					} else {
						printf("--- Siguiente ronda ---\n");
						usleep(ESPERAUS*1000); 
						for(i=0; i<CANTIDAD; i++){
							enviar_mensaje(id_cola, MSG_ANIMAL+i, MSG_PISTA, EVT_CORRO, "");
						}
						respuestas_recibidas = 0;
						/* Resetear el array de ganadores para la próxima ronda */
						for(i = 0; i < CANTIDAD; i++){
							ganadores_ronda[i] = FALSE;
						}
					}
				}
			break;
			case EVT_FIN:
				sscanf(msg.contenidoMsg, "%s %d %d", nombre_animal, &cant_pasos, &cont_ronda);
				
				for(i=0; i<CANTIDAD; i++){
					enviar_mensaje(id_cola, MSG_ANIMAL+i, MSG_PISTA, EVT_FIN, msg.contenidoMsg);
				}
				fin= TRUE;
			break;
		}
	}

	return 0;
}
