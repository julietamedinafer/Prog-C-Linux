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

	int id_auto, ganador, meta, posicion, cont_avance, puntos, cont_rtas= 0;
	int i;
	int cant_participantes, participantes_activos;
	int fin= FALSE;
	char msg_enviar[TAMANO_BUFFER];
	char linea[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();
	borrar_mensaje(id_cola);

	/* semaforo */
	id_semaforo= creo_semaforo();
	inicia_semaforo(id_semaforo, VERDE);

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

	printf("\nBIENVENIDO\n\n");

	meta= 295;
	printf("El recorrido tiene %dm\n", meta);
	
	/* señal a los participantes para iniciar */
	for(i= 0; i< cant_participantes; i++){
		sprintf(msg_enviar, "%d %d", meta, cant_participantes);
		enviar_mensaje(id_cola, MSG_PARTICIPANTE+i, MSG_LUGAR, EVT_INICIO, msg_enviar);
	}

	/* carrera */
	while(fin== FALSE){
		recibir_mensaje(id_cola, MSG_LUGAR, &msg);
		id_auto= msg.remitente - MSG_PARTICIPANTE;

		switch(msg.evento){
			case EVT_NINGUNO:
				sprintf(msg_enviar, "%d", participantes_activos);
				enviar_mensaje(id_cola, MSG_PARTICIPANTE+id_auto, MSG_LUGAR, EVT_AVANZAR, msg_enviar);
				usleep(ESPERAUS*1000);
			break;

            case EVT_INFO_CARRERA:
                sscanf(msg.contenidoMsg, "%d %d", &posicion, &cont_avance);
				
				if(posicion== -1){
					printf("Auto %d eliminado!\n\n", id_auto+1);
					participantes_activos--;
				}else if(posicion== -2){
					printf("Auto %d en boxes!\n\n", id_auto+1);
				}else{
					printf("Auto %d avanzo %dm\nTotal recorrido %dm\n\n", id_auto+1, posicion, cont_avance);
				}
					
				memset(msg_enviar, 0x00, sizeof(msg_enviar));
				sprintf(msg_enviar, "%d", participantes_activos);
				enviar_mensaje(id_cola, MSG_PARTICIPANTE+id_auto, MSG_LUGAR, EVT_AVANZAR, msg_enviar);
				usleep(ESPERAUS*1000);

            break;

            case EVT_GANADOR:
                sscanf(msg.contenidoMsg, "%d %d", &ganador, &puntos);
                printf("RESULTADO FINAL: El auto %d gano la carrera con %d puntos!\n", ganador, puntos);
                fin= TRUE;
            break;
		}
		
		usleep(ESPERAUS*1000);
	}

	/* limpieza de archivos de sincronizacion */
	remove(ARCHIVO);
	remove(FLAG_UNO);
	remove(FLAG_DOS);

	/* limpieza de cola de mensajes */
	borrar_cola(id_cola);

	return 0;
}
