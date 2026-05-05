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
#include "archivos.h"
#include "semaforo.h"


int main(int argc,char * argv[]){

	int id_caja, precio, cant_productos, total= 0;
	char descripcion[TAMANO_BUFFER];
    char nombre_archivo[TAMANO_BUFFER];
	int i; 
	int fin= FALSE;
	char msg_enviar[TAMANO_BUFFER];
	mensaje msg; /* estructura del mensaje en cola.h */

	/* semaforo  */
	id_semaforo= creo_semaforo();
	inicia_semaforo(id_semaforo,VERDE);

	/* aleatorio */
	inicializar_aleatorio();

	/* cola */
	id_cola= creo_id_cola();

	/* limpieza */
	borrar_mensaje(id_cola); 
	memset(&msg, 0x00, sizeof(mensaje));
	memset(msg_enviar, 0x00, sizeof(msg_enviar));
	memset(descripcion, 0x00, sizeof(descripcion));

	printf("\nBIENVENIDO AL SUPER\n\n");
	
	/* señal a los participantes para iniciar */
	for(i= 0;i< CANTIDAD;i++){
		enviar_mensaje(id_cola, MSG_CAJAS+i, MSG_SUPER, EVT_INICIO, "");
	}

	while(fin== FALSE){ 
		precio = 0;
		total= 0;
		cant_productos= obtener_aleatorio(1, 20);
		printf("Cantidad de productos %d\n", cant_productos);
		for(i=0; i<cant_productos; i++){
			printf("Ingrese descripcion del producto: ");
			scanf("%s", descripcion);
			precio= obtener_aleatorio(100, 1000);
			total+= precio;
			printf("precio producto %d - total %d\n", precio, total);
		}

		if(cant_productos>= 1 && cant_productos<= 5){
			id_caja = 0; /* Caja 1 (indice 0) */ 
		}
		else if(cant_productos>= 6 && cant_productos<= 10){
			id_caja = 1; /* Caja 2 (indice 1) */
		}
		else if(cant_productos>= 11 && cant_productos<= 20){
			id_caja = 2; /* Caja 3 (indice 2) */
		}

		printf("Cliente derivado a CAJA %d\n", id_caja+1);

		memset(msg_enviar, 0x00, sizeof(msg_enviar));
		sprintf(msg_enviar, "%d", total);
		enviar_mensaje(id_cola, MSG_CAJAS+id_caja, MSG_SUPER, EVT_COBRAR, msg_enviar);

		usleep(ESPERAUS*1000);
		printf("Continuar atendiendo?\n0)Si\n1)No: ");
		scanf("%d", &fin);
		
		if(fin==1){
			fin= TRUE;
			for(i=0;i< CANTIDAD;i++){
				enviar_mensaje(id_cola, MSG_CAJAS+i, MSG_SUPER, EVT_FIN, "");
			}
		}
	}

	return 0;
}
