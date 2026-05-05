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
#include "archivos.h"

 
int main(int argc,char * argv[]){

	int id_jugador, posicion, cont_avance, vivo, camino_elegido, meta_final;
	int avance_criatura, ataque_criatura;
	int i, j;
	int cant_participantes, participantes_activos;
	int fin= FALSE, criaturas_ganaron= FALSE;
	int jugadores_eliminados[4]= {0}; 
	char msg_enviar[TAMANO_BUFFER];
	char linea[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */
	Criatura criaturas[2];

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();
	borrar_mensaje(id_cola);

	/* semaforo */
	id_semaforo= creo_semaforo();
	inicia_semaforo(id_semaforo, VERDE);

	/* inicializar criaturas */
	for(i=0; i<2; i++){
		criaturas[i].id= i;
		criaturas[i].posicion= 0;
		criaturas[i].activa= 1;
	}

	/* limpieza */
	borrar_mensaje(id_cola); 
	memset(&msg, 0x00, sizeof(mensaje));
	memset(msg_enviar, 0x00, sizeof(msg_enviar));
	memset(linea, 0x00, sizeof(linea));

	/* sincronizacion bidireccional */
	espera_semaforo(id_semaforo);
	abrirArchivo(FLAG_UNO, "w");
	escribirArchivo("listo");
	cerrarArchivo();
	levanta_semaforo(id_semaforo);

	/* esperar al otro proceso*/
	while(1){
		espera_semaforo(id_semaforo);
		if(abrirArchivo(FLAG_DOS, "r")==0){
			cerrarArchivo();
			if(abrirArchivo(ARCHIVO, "r")==0){
				leerArchivoPalabra(linea);
				cant_participantes= atoi(linea);
				cerrarArchivo();
				levanta_semaforo(id_semaforo);
				break;
			}
		}
		levanta_semaforo(id_semaforo);
		usleep(100 * 1000);
	}

	participantes_activos= cant_participantes;

	printf("\nBIENVENIDO AL LABERINTO\n\n");
	
	meta_final= 150; 

	/* señal a los jugadores para iniciar */
	for(i= 0; i< cant_participantes; i++){
		memset(msg_enviar, 0x00, sizeof(msg_enviar));
		sprintf(msg_enviar, "%d %d", meta_final, cant_participantes); 
		enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_LABERINTO, EVT_INICIO, msg_enviar);
	}

	while(fin== FALSE && criaturas_ganaron== FALSE){
		for(i=0; i<2; i++){
			if(criaturas[i].activa){
				avance_criatura= obtener_aleatorio(2, 4);
				criaturas[i].posicion+= avance_criatura;
				printf("Criatura %d avanza %dm, posicion total: %dm\n", i+1, avance_criatura, criaturas[i].posicion);
				
				/* verificar si las criaturas llegaron primero */
				if(criaturas[i].posicion >= 150){
					printf("LAS CRIATURAS GANARON!!! La criatura %d llego primero.\n", i+1);
					printf("Todos los muchachos regresan a la isla para siempre\n");
					criaturas_ganaron= TRUE;
					
					/* avisar a todos los jugadores que perdieron */
					for(j= 0; j< cant_participantes; j++){
						memset(msg_enviar, 0x00, sizeof(msg_enviar));
						sprintf(msg_enviar, "%d", -1);
						enviar_mensaje(id_cola, MSG_JUGADOR+j, MSG_LABERINTO, EVT_FIN, msg_enviar);
					}
					
					usleep(ESPERAUS*1000);
				}
			}
		}
		
		/* procesar turnos de jugadores */
		for(i=0; i<cant_participantes; i++){
			ataque_criatura= obtener_aleatorio(1, 2);
			memset(msg_enviar, 0x00, sizeof(msg_enviar));
			sprintf(msg_enviar, "%d", ataque_criatura);
			enviar_mensaje(id_cola, MSG_JUGADOR+i, MSG_LABERINTO, EVT_ATAQUE_CRIATURA, msg_enviar);
		}
		
		/* recibir respuestas de todos los jugadores */
		for(i=0; i<cant_participantes; i++){
			recibir_mensaje(id_cola, MSG_LABERINTO, &msg);
			id_jugador= msg.remitente - MSG_JUGADOR;
			
			switch(msg.evento){
				case EVT_AVANZAR:
					sscanf(msg.contenidoMsg, "%d %d %d %d %d", &vivo, &posicion, &cont_avance, &camino_elegido, &meta_final);
					
					if(vivo== 0){
						if(jugadores_eliminados[id_jugador]== 0){
							printf("Muchacho %d fue eliminado y regresa a la isla\n\n", id_jugador+1);
							participantes_activos--;
							jugadores_eliminados[id_jugador]= 1; /* marcar como ya eliminado */
						}
					}else{
						printf("Muchacho %d avanzo %dm, posicion total: %dm", id_jugador+1, posicion, cont_avance);
						if(camino_elegido == 3){
							printf("camino largo de 180");
						}
						printf("\n\n");
						
						if(cont_avance>= meta_final && vivo== 1){
							printf("MUCHACHO %d ESCAPO DEL LABERINTO!!!\n", id_jugador+1);
							
							/* avisar a todos los otros que perdieron */
							for(j= 0; j< cant_participantes; j++){
								if(j!= id_jugador){
									sprintf(msg_enviar, "%d", id_jugador+1);
									enviar_mensaje(id_cola, MSG_JUGADOR+j, MSG_LABERINTO, EVT_FIN, msg_enviar);
								}
							}
							
							fin= TRUE;
						}
					}
				break;
			}
		}
		
		if(fin == TRUE){
			usleep(ESPERAUS*1000);
		}
		
		/* verificar si quedan jugadores */
		if(participantes_activos<= 0){
			printf("Todos los muchachos fueron eliminados. Las criaturas ganaron.\n");
			
			/* avisar a todos los threads que terminen */
			for(j= 0; j< cant_participantes; j++){
				memset(msg_enviar, 0x00, sizeof(msg_enviar));
				sprintf(msg_enviar, "%d", -1);
				enviar_mensaje(id_cola, MSG_JUGADOR+j, MSG_LABERINTO, EVT_FIN, msg_enviar);
			}
			
			usleep(ESPERAUS*1000);
			fin= TRUE;
		}
		
		usleep(ESPERAUS*2000);
	}

	/* limpieza de archivos de sincronizacion */
	remove(ARCHIVO);
	remove(FLAG_UNO);
	remove(FLAG_DOS);

	/* limpieza de cola de mensajes */
	borrar_cola(id_cola);
	
	/* limpieza de semaforo */
	destruir_semaforo(id_semaforo);

	return 0;
}
