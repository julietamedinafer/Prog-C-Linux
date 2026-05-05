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

	int id_auto, ganador, meta, posicion, cant_pasos;
	int i;
	int cant_autos;
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
				cant_autos= atoi(linea);
				cerrarArchivo();
				levanta_semaforo(id_semaforo);
				break;
			}
		}
		levanta_semaforo(id_semaforo);
		usleep(100 * 1000);
	}


	printf("\nBIENVENIDO\n\n");

	meta= obtener_aleatorio(100, 1000);
	printf("El recorrido tiene %dm\n", meta);
	
	/* señal a los participantes para iniciar */
	for(i= 0;i< cant_autos;i++){
		sprintf(msg_enviar, "%d %d", meta, cant_autos);
		enviar_mensaje(id_cola, MSG_AUTO+i, MSG_CIRCUITO, EVT_INICIO, msg_enviar);
	}

	/* carrera */
	while(fin== FALSE){
		recibir_mensaje(id_cola, MSG_CIRCUITO, &msg);
		id_auto= msg.remitente - MSG_AUTO;

		switch(msg.evento){
			case EVT_NINGUNO:
				printf("Auto %d en posición\n\n", id_auto+1);
				enviar_mensaje(id_cola, MSG_AUTO+id_auto, MSG_CIRCUITO, EVT_AVANZAR, "");
			break;

            case EVT_AVANZAR:
                memset(msg_enviar, 0x00, sizeof(msg_enviar));
                sscanf(msg.contenidoMsg, "%d %d", &posicion, &cant_pasos);
                printf("Auto %d avanzo %dm\nTotal recorrido %dm\n\n", id_auto+1, posicion, cant_pasos);
                
                /* enviar informacion de la carrera al auto */
                sprintf(msg_enviar, "Circuito informa: Distancia restante aprox %dm", meta - cant_pasos);
                enviar_mensaje(id_cola, MSG_AUTO+id_auto, MSG_CIRCUITO, EVT_INFO_CARRERA, msg_enviar);
                
                enviar_mensaje(id_cola, MSG_AUTO+id_auto, MSG_CIRCUITO, EVT_AVANZAR, "");
            break;

            case EVT_GANADOR:
                sscanf(msg.contenidoMsg, "%d", &ganador);
                printf("RESULTADO FINAL: El auto %d gano la carrera!\n", ganador);
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
