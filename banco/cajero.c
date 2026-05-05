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
	
	int opcion;
	int codigo, importe;
	mensaje msg; /* estructura del mensaje en cola.h */

	/* cola */
	id_cola= creo_id_cola();

	/* aleatorio */
	inicializar_aleatorio();

	/* limpieza */
	memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));

	/* mensaje de bienvenida */
	if(argc<2){
		printf("\nError: Debe ingresar el codigo de cliente como parametro. Ejemplo: ./cajero 1\n");
		return 0;
	}
	msg.remitente= atoi(argv[1]);	
	printf("\nBIENVENIDO A CAJERO %d\n\n", msg.remitente);

	/* menu*/
	printf("\nQue operacion desea realizar?:\n0) Salir\n1) Consultar saldo\n2) Depositar dinero\n3) Extraer dinero\n");
	scanf("%d", &opcion); 

	while(opcion!=0){
		msg.tipo_msg= TIPO_BANCO; /* todos los mensajes al banco van con este tipo */

		printf("\nIngrese codigo de cliente (entre 1 y 100): \n");
		scanf("%d", &codigo);
	
		switch(opcion){
			case 1:
				msg.evento= CONSULTA_SALDO;
				memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
				sprintf(msg.contenidoMsg, "%d", codigo);
				printf("\nConsultando saldo\n");
				break;
			case 2:
				msg.evento= DEPOSITO;
				memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
				printf("\nIngrese el importe a depositar: \n");
				scanf("%d", &importe);
				sprintf(msg.contenidoMsg, "%d %d", codigo, importe);
				printf("\nDepósito en proceso\n");
				break;
			case 3:
				msg.evento= EXTRACCION;
				memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
				printf("\nIngrese el importe a extraer: \n");
				scanf("%d", &importe);
				sprintf(msg.contenidoMsg, "%d %d", codigo, importe);
				printf("\nExtracción en proceso\n");
				break;
		}
		
		enviar_mensaje(id_cola, msg.tipo_msg, msg.remitente, msg.evento, msg.contenidoMsg);

		recibir_mensaje(id_cola, msg.remitente, &msg);
		
		switch(msg.evento){
			case RTA_SALDO:
				printf("\n%s\n", msg.contenidoMsg);
				break;
			case RTA_SALDO_E:
				printf("\n%s\n", msg.contenidoMsg);
				break;
			case RTA_DEPOSITO:
				printf("\n%s\n", msg.contenidoMsg);
				break;
			case RTA_EXTRACCION:
				printf("\n%s\n", msg.contenidoMsg);
				break;
			case RTA_EXTRACCION_E:
				printf("\n%s\n", msg.contenidoMsg);
				break;
		}

		printf("\nQue operacion desea realizar?:\n0) Salir\n1) Consultar saldo\n2) Depositar dinero\n3) Extraer dinero\n");
		scanf("%d", &opcion);
	}	
	
	borrar_mensaje(id_cola);

	return 0;
}
