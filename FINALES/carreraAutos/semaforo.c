#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "def.h"
#include "semaforo.h"
#include "clave.h"
#include <sys/sem.h>

int creo_semaforo()
{
	key_t clave;
	int id_semaforo;	
	clave = creo_clave();
	id_semaforo = semget(clave,1,0600|IPC_CREAT); 

	if(id_semaforo==-1)
	{
		printf("error: no se pudo crear el semaforo\n");
		exit(0);	
	}
	return id_semaforo;
}

void inicia_semaforo(int id_semaforo,int valor)
{
	semctl(id_semaforo,0,SETVAL,valor);
}

void levanta_semaforo(int id_semaforo)
{
	struct sembuf operacion;
	/*printf("Levanta SEMAFORO\n");	*/
	operacion.sem_num = 0;
	operacion.sem_op = 1; /*incrementa el semaforo en 1*/
	operacion.sem_flg = 0;
	semop(id_semaforo,&operacion,1);
}

void espera_semaforo(int id_semaforo)
{
	struct sembuf operacion;
	/*printf("Espera SEMAFORO\n");*/
	operacion.sem_num = 0;
	operacion.sem_op = -1;
	operacion.sem_flg = 0;
	semop(id_semaforo,&operacion,1);
}
