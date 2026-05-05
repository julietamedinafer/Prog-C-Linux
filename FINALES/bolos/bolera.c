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

/*CONSIGNA:
Proceso BOLERA (MSG_ BOLERA)
Hilo Principal (main):
1) La cantidad de jugadores es aleatoria de 1 a 3 jugadores.
2) Borrar los mensajes anteriores, para empezar de 0.
3) Inicializa una vector local de N (numero de jugadores) posiciones, uno por cada línea de
bolos que le corresponde a cada jugador. Ambas posiciones se inicializan con la cantidad
de 10 pinos.
4) Le envía un mensaje a cada JUGADOR, EVT_INICIO. (MSG_JUGADOR+i).
5) Espera recibir mensajes de los JUGADORES.
Si recibe EVT_TIRO.
• Define al azar los pinos tirados del 1 hasta la cantidad de pinos disponibles
(cuidado: utilizar la línea correcta dependiendo del jugador) • Actualiza el vector con la posición correcta con la cantidad de pinos en pie. • Si se tiran todos los pinos de una vez, muestra la leyenda “CHUZA”. • Devuelve la cantidad de pinos tirados al jugador con el EVT_RESULTADO,
cantidad_de_pinos. • Si se tiran todos los pinos o ya recibió 3 tiros de ese jugador, envía el EVT_FIN al
jugador correspondiente (esto lo hace en lugar de EVT_RESULTADO) con la
cantidad de puntos obtenidos (1 por cada pino tirado y 20 si se tiran todos de una).
6) Si terminaron de tirar los N jugadores (o bien tiraron todos los pinos o tiran 3 veces),
finaliza.
7) Caso contrario, repite desde el punto 5.*/


int main(int argc,char * argv[]){

	int id_participante, meta, cant_bolos= 0; 
	int i;
	int cant_participantes;
	int array_bolos[3]; 
	int array_tiros[3] = {0, 0, 0};  /* contador de tiros por participante */
	int array_puntos[3] = {0, 0, 0}; /* puntos por participante */
	int participantes_terminados= 0;
	int fin= FALSE;
	char msg_enviar[TAMANO_BUFFER];
	char linea[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();

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


	printf("\nBIENVENIDO\n\n");

	meta= CANTIDAD;
	printf("Bolos totales: %d\n", meta);

	/* inicializar array de bolos */
	for(i= 0; i< cant_participantes; i++){
		array_bolos[i]= meta;
	}
	
	/* señal a los participantes para iniciar */
	for(i= 0;i< cant_participantes;i++){ 
		sprintf(msg_enviar, "%d", meta);
		enviar_mensaje(id_cola, MSG_PARTICIPANTE+i, MSG_LUGAR, EVT_INICIO, msg_enviar);
	}

	/* carrera */
	while(fin== FALSE){
		recibir_mensaje(id_cola, MSG_LUGAR, &msg);
		id_participante= msg.remitente - MSG_PARTICIPANTE;

		switch(msg.evento){
            case EVT_TIRO:
				if(array_bolos[id_participante]> 0 && array_tiros[id_participante]< 3){
					cant_bolos= obtener_aleatorio(1, array_bolos[id_participante]);
					array_bolos[id_participante]-= cant_bolos;
					array_tiros[id_participante]++;
					array_puntos[id_participante]+= cant_bolos;
					
					if(array_bolos[id_participante]== 0){
						if(array_tiros[id_participante]== 1){
							printf("CHUZA! El participante %d tiro todos los bolos de una vez!\n", id_participante+1);
							array_puntos[id_participante]= 20;
						}else if(array_tiros[id_participante]>= 3){
							printf("El participante %d termino. Tiros: %d, Puntos totales: %d\n", id_participante+1, array_tiros[id_participante], array_puntos[id_participante]);
						}
						memset(msg_enviar, 0x00, sizeof(msg_enviar));
						sprintf(msg_enviar, "%d %d", cant_bolos, array_puntos[id_participante]);
						enviar_mensaje(id_cola, MSG_PARTICIPANTE+id_participante, MSG_LUGAR, EVT_FIN, msg_enviar);
						participantes_terminados++;
					}
					else{
						printf("El participante %d tiro %d bolos. Quedan %d bolos.\n", id_participante+1, cant_bolos, array_bolos[id_participante]);
						memset(msg_enviar, 0x00, sizeof(msg_enviar));
						sprintf(msg_enviar, "%d %d", cant_bolos, array_puntos[id_participante]);
						enviar_mensaje(id_cola, MSG_PARTICIPANTE+id_participante, MSG_LUGAR, EVT_RESULTADO, msg_enviar);

					}
				}
				
				if(participantes_terminados>= cant_participantes){
					printf("\n=== JUEGO TERMINADO ===\n");
					for(i = 0; i < cant_participantes; i++){
						printf("Participante %d: %d puntos\n", i+1, array_puntos[i]);
					}
					fin = TRUE;
				}
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
