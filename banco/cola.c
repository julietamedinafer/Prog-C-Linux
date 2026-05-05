#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "def.h"
#include "clave.h"
#include "cola.h"

int creo_id_cola(){
	key_t clave;
	int id_cola;
	clave= creo_clave();
	id_cola= msgget(clave, 0600 | IPC_CREAT);
	if (id_cola== -1){
		printf("Error al obtener identificador para cola de mensajes\n");
		exit(-1);
	}
	return id_cola;
}

int enviar_mensaje(int id_cola, long receptor, int emisor, int eventoMandar, char* contenidoMsgMandar){
	mensaje msg;
	msg.tipo_msg= receptor;
	msg.remitente= emisor;
	msg.evento= eventoMandar;
	strcpy(msg.contenidoMsg, contenidoMsgMandar);

	return msgsnd(id_cola, (struct msgbuf *)&msg, sizeof(msg.remitente)+sizeof(msg.evento)+sizeof(msg.contenidoMsg), IPC_NOWAIT);
}

int recibir_mensaje(int id_cola, long destino, mensaje* msgRecibir){
	mensaje msg;
	int res;
	res= msgrcv(id_cola, (struct msgbuf *)&msg, sizeof(msg.remitente)+sizeof(msg.evento)+sizeof(msg.contenidoMsg), destino, 0);
	msgRecibir->tipo_msg= msg.tipo_msg;
	msgRecibir->remitente= msg.remitente;
	msgRecibir->evento= msg.evento;
	strcpy(msgRecibir->contenidoMsg, msg.contenidoMsg);

	return res;
}

int borrar_mensaje(int id_cola){
	mensaje msg;
	int res;
	do{
		res= msgrcv(id_cola, (struct msgbuf*)&msg, sizeof(msg.remitente) + sizeof(msg.evento) + sizeof(msg.contenidoMsg), 0, IPC_NOWAIT);}while(res>0);

	return res;
}

int borrar_cola(int id_cola){
	msgctl(id_cola, IPC_RMID, (struct msqid_ds*)NULL);

	return 0;
}

		


