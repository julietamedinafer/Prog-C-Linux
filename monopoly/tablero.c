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
#include "semaforo.h"
#include "memoria.h"


int main(int argc,char * argv[]){

	int id_jugador, posicion, saldo, ganador, perdio;
	int i, respuestas_recibidas= 0, ronda_actual= 0;
	int jugadores_activos, cant_jugadores, jugadores_iniciados= 0;
	int jugador_ganador= -1, max_saldo= -1;
	int fin= FALSE;
	char msg_enviar[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */
	Sincronizacion *memoria;

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();

	/* sincronizacion entre procesos */
	memoria= (Sincronizacion*)creo_memoria(sizeof(Sincronizacion), &id_memoria, IDKEY);

	/* semaforo */
	id_semaforo= creo_semaforo();
	inicia_semaforo(id_semaforo, VERDE);

	/* limpieza */
	borrar_mensaje(id_cola); 
	memset(&msg, 0x00, sizeof(mensaje));
	memset(msg_enviar, 0x00, sizeof(msg_enviar));

	/* sincronizacion entre procesos parte 2*/
	espera_semaforo(id_semaforo);
	memoria->proceso_1_preparado= 1;
	levanta_semaforo(id_semaforo);
	
	while(1){
		espera_semaforo(id_semaforo);
		if(memoria->proceso_2_preparado== 1){
			cant_jugadores = memoria->cant_jugadores;
			levanta_semaforo(id_semaforo);
			break;
		}
		levanta_semaforo(id_semaforo);
		usleep(100 * 1000);
	}

	printf("\nBIENVENIDO\n\n");
	
	/* señal a los participantes para iniciar */
	for(i= 0;i< cant_jugadores;i++){
		enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_TABLERO, EVT_INICIO, "");
	}

	/* esperar confirmacion de inicio de todos los jugadores */
	while(jugadores_iniciados< cant_jugadores){
		recibir_mensaje(id_cola, MSG_TABLERO, &msg);
		id_jugador= msg.remitente - MSG_JUGADOR;
		if(msg.evento== EVT_INICIO){
			jugadores_iniciados++;
			printf("\nJugador %d confirmado\n", id_jugador+1);
		}
	}
	printf("Todos los jugadores han confirmado la finalizacion\n");
	

	jugadores_activos= cant_jugadores;

	while(fin== FALSE && jugadores_activos> 1){
		printf("\nRONDA %d\n", ronda_actual+1);
		respuestas_recibidas= 0;

		for(i= 0; i< cant_jugadores; i++){
			enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_TABLERO, EVT_CORRO, "");
		}
		
		while(respuestas_recibidas< cant_jugadores && fin== FALSE){
			recibir_mensaje(id_cola, MSG_TABLERO, &msg);
			id_jugador= msg.remitente - MSG_JUGADOR;
			
			if(msg.evento== EVT_CORRO){
				sscanf(msg.contenidoMsg, "%d %d %d %d", &posicion, &saldo, &ganador, &perdio);
				
				printf("Jugador %d esta en posicion %d con saldo $%d\n", id_jugador+1, posicion, saldo);
			
				if(ganador== TRUE || posicion >= 60){
					printf("EL jugador %d gano\n", id_jugador+1);
					jugador_ganador= id_jugador;
					max_saldo= saldo;
					fin= TRUE;
					break;
				}else if(perdio== TRUE || saldo<= 0){
					printf("EL jugador %d perdio\n", id_jugador+1);
					jugadores_activos--;
				}else{
					if(saldo> max_saldo){
						max_saldo= saldo;
						jugador_ganador= id_jugador;
					}
				}
				
				respuestas_recibidas++;
			}
		}
		
		printf("Fin de la ronda %d\n", ronda_actual + 1);
		printf("Jugadores activos: %d\n\n", jugadores_activos);
		
		ronda_actual++;
		
		if(jugadores_activos<= 1 && fin== FALSE){
			printf("solo queda un jugador activo. fin del juego!\n");
			fin= TRUE;
		}
		
		usleep(ESPERAUS*1000);
	}

	/* enviar señal de fin a todos los jugadores */
	for(i= 0;i< cant_jugadores;i++){
		enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_TABLERO, EVT_FIN, "");
	}

	printf("\n\nRESULTADO FINAL\n\n");
	if(jugador_ganador>= 0){
		printf("El ganador es el jugador %d con $%d!\n", jugador_ganador+1, max_saldo);
	}else{
		printf("No hay ganador claro\n");
	}

	libero_memoria((char *)memoria, id_memoria);
	return 0;
}
