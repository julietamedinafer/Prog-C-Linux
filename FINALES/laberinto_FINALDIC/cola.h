#ifndef _COLA
#define _COLA

#include "def.h" /* AGREGADO PORQUE DEPENDE DEL ORDEN DE INCLUSION DE LOS HEADERS PARA FUNCIONAR */
#include "clave.h"

/* ESTRUCTURA DE MI MENSAJE 
!!!IMPORTANTE LA PRESENCIA DEL 'TIPO DE MENSAJE' EN TIPO DE DATO 'LONG'*/

typedef struct mensaje{
	long tipo_msg;
	int remitente;
	int evento;
	char contenidoMsg[100+1];
}mensaje;

int creo_id_cola();
int enviar_mensaje(int id_cola, long receptor, int emisor, int eventoMandar, char* contenidoMsgMandar);
int recibir_mensaje(int id_cola, long destino, mensaje *msgRecibir);
int borrar_mensaje(int id_cola);
int borrar_cola(int id_cola);

#endif
