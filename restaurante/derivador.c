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

/*CONSIGNA:
El resto debe mostrar cuantas comidas se fueron consumiendo, y si tuvo postre o no. 
Y el total de cada comida pedida con su precio.*/

int main(int argc,char * argv[]){

	int cant_pedidos= 0, total= 0;
	int pizzas= 0, hamburguesas= 0, panchos= 0, postres= 0;
	int id_mesero, continuar= 0;
	Comida com;
	int i;
	int fin= FALSE;
	char msg_enviar[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */
	Sincronizacion *memoria;

	/* aleatorio */

	/* cola */
	id_cola= creo_id_cola();

	/* sincronizacion entre procesos */
	memoria= (Sincronizacion*)creo_memoria(sizeof(Sincronizacion), &id_memoria, IDKEY);

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
			levanta_semaforo(id_semaforo);
			break;
		}
		levanta_semaforo(id_semaforo);
		usleep(100 * 1000);
	}

	printf("\nBIENVENIDO\n\n");
	
	/* señal a los participantes para iniciar */
	for(i= 0;i< CANTIDAD;i++){
		enviar_mensaje(id_cola, MSG_MESERO+i, MSG_RESTO, EVT_INICIO, "");
	}

	while(fin== FALSE){
		recibir_mensaje(id_cola, MSG_RESTO, &msg);
		id_mesero= msg.remitente - MSG_MESERO;

		switch(msg.evento){
			case EVT_PREPARAR:
				cant_pedidos++;
				sscanf(msg.contenidoMsg, "%d %s %d", &com.precio, com.descripcion, &com.postre);
				
				total+= com.precio;
				if(strcmp(com.descripcion, "Pizza")== 0){
					pizzas++;
				}else if(strcmp(com.descripcion, "Hamburguesa")== 0){
					hamburguesas++;
				}else if(strcmp(com.descripcion, "Pancho")== 0){
					panchos++;
				}

				if(com.postre== 1){
					postres++;
					printf("Mesero %d: Se preparo %s con postre\n", id_mesero+1, com.descripcion);
				}else{
					printf("Mesero %d: Se preparo %s sin postre\n", id_mesero+1, com.descripcion);
				}
				
				enviar_mensaje(id_cola, MSG_MESERO+id_mesero, MSG_RESTO, EVT_LISTO, "");

				printf("\n\nRESUMEN PEDIDOS\n\n");
				printf("Cantidad de pedidos: %d | Total recaudado: $%d\n", cant_pedidos, total);
				printf("Pizzas: %d | Hamburguesas: %d | Panchos: %d | Postres: %d\n", pizzas, hamburguesas, panchos, postres);
				
				printf("\nTomar mas pedidos?\n1)Si\n2)No:\n");
				scanf(" %d", &continuar);

				if(continuar== 2){
					for(i= 0;i< CANTIDAD;i++){
						enviar_mensaje(id_cola, MSG_MESERO+i, MSG_RESTO, EVT_FIN, "");
					}
					fin = TRUE;
				}else{
					usleep(ESPERAUS*1000);
					enviar_mensaje(id_cola, MSG_MESERO+id_mesero, MSG_RESTO, EVT_INICIO, "");
				}
			break;
			case EVT_FIN:
				printf("\nEL RESTAURANTE CERRO CON:\n");
				printf("Total de pedidos procesados: %d\n", cant_pedidos);
				printf("Total recaudado: $%d\n", total);
				printf("Pizzas vendidas: %d\n", pizzas);
				printf("Hamburguesas vendidas: %d\n", hamburguesas);
				printf("Panchos vendidos: %d\n", panchos);
				printf("Postres vendidos: %d\n", postres);
				fin= TRUE;
			break;
		}
	}

	libero_memoria((char *)memoria, id_memoria);
	return 0;
}
