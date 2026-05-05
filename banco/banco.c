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

	int i=0;
	int saldos[HASTA_CLIENTE]= {0}; /* incializo todos los saldos en 0 */
	mensaje msg;
	Cliente cli;

	/* cola */
	id_cola= creo_id_cola();

	/* aleatorio */
	inicializar_aleatorio();

	/* limpieza */
	borrar_mensaje(id_cola);  

	printf("\nBIENVENIDO AL BANCO\n");

	while(1){
		recibir_mensaje(id_cola, TIPO_BANCO, &msg);
		sscanf(msg.contenidoMsg, "%d %d", &cli.codigo, &cli.saldo);

		if(cli.codigo<1 || cli.codigo>100){
			memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
			sprintf(msg.contenidoMsg, "\nCodigo de cliente invalido\n");
			printf("\nCodigo de cliente invalido\n");
			msg.evento= RTA_SALDO_E;
		}else{
			if(saldos[cli.codigo-1]==0){
				saldos[cli.codigo-1]=obtener_aleatorio(DESDE_SALDO, HASTA_SALDO);
			}
			switch(msg.evento){
				case CONSULTA_SALDO:
					cli.saldo= saldos[cli.codigo-1];
					memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
					sprintf(msg.contenidoMsg, "\nCliente %d | Saldo %d\n", cli.codigo, cli.saldo);
					printf("\n%s\n",msg.contenidoMsg);
					msg.evento= RTA_SALDO;
					break;
				case DEPOSITO:
					saldos[cli.codigo-1]+= cli.saldo;
					memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
					sprintf(msg.contenidoMsg, "\nCliente %d | Saldo actualizado: %d\n", cli.codigo, saldos[cli.codigo-1]);
					printf("\n%s\n",msg.contenidoMsg);
					msg.evento= RTA_DEPOSITO;
					break;
				case EXTRACCION:
					if(saldos[cli.codigo-1]< cli.saldo){
						memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
						sprintf(msg.contenidoMsg, "\nCliente %d | Saldo insuficiente | Saldo actual: %d\n", cli.codigo, saldos[cli.codigo-1]);
						printf("\n%s\n",msg.contenidoMsg);
						msg.evento= RTA_EXTRACCION_E;
					}else{
						saldos[cli.codigo-1]-= cli.saldo;
						memset(msg.contenidoMsg, 0x00, sizeof(msg.contenidoMsg));
						sprintf(msg.contenidoMsg, "\nCliente %d | Saldo actualizado: %d\n", cli.codigo, saldos[cli.codigo-1]);
						printf("\n%s\n",msg.contenidoMsg);
						msg.evento= RTA_EXTRACCION;
					}
					break;
			}
		}
		
		enviar_mensaje(id_cola, msg.remitente, msg.tipo_msg, msg.evento, msg.contenidoMsg);
	}

	borrar_cola(id_cola);

	return 0;
}
