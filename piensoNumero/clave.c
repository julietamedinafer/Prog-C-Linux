#include "stdio.h"
#include "stdlib.h"
#include "clave.h"
#include "def.h"
#include <sys/ipc.h>

key_t creo_clave(){

	key_t clave;
	clave= ftok(PATHKEY, IDKEY);
	if(clave== (key_t) - 1){
		printf("No puedo conseguir clave semaforo\n");
		exit(0);
	}

	return clave;
}
