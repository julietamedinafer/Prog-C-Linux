#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "funciones.h"
#include "global.h"
#include "definiciones.h"
#include "archivos.h"
#include "semaforos.h"
/*#include "clave.h" solo usar si paso clave a semaforo */ 

int main(int argc, char *argv[]) {
	char almacenar[50],nuevoNombre[50],almacenar2[50],anterior[50]; 
	int panelnro;
	FILE *carga; 
	int id_semaforo;

		if (argc < 2) {
        printf("ingrese el numero de panel \n");
        return 1;
    }
	panelnro = atoi(argv[1]); 

	if (panelnro < 1 || panelnro > PANELES){
		printf("ingrese cajeros del 1 al %i\n", PANELES);
        return 1;
	}

	memset(nuevoNombre, 0, sizeof(nuevoNombre));
	memset(anterior, 0, sizeof(anterior));
	memset(almacenar, 0, sizeof(almacenar));
	memset(almacenar2, 0, sizeof(almacenar2));
		
	id_semaforo = creo_semaforo();

	while(1){
		
		
		
			espera_semaforo(id_semaforo);
			memset(nuevoNombre, 0, sizeof(nuevoNombre));
			sprintf(nuevoNombre, "panel%i.dat",panelnro);
			carga = inAbrirArchivo(nuevoNombre, "r");
			
			if (carga!=NULL){
				inLeerArchivo(almacenar);
				if (atoi(almacenar) == panelnro){
					inLeerArchivo(almacenar2);
					if (strcmp(almacenar2,anterior) != 0){
						strcpy(anterior,almacenar2);
						system("clear");
						printf("el mensaje es %s \n",almacenar2 );
					}
				}
				
				voCerrarArchivo();
				
			
	
				memset(almacenar, 0, sizeof(almacenar));
				memset(almacenar2, 0, sizeof(almacenar2));
				memset(anterior, 0, sizeof(anterior));
			}

			levanta_semaforo(id_semaforo);
			usleep(100000);
	}
		
	return 0;

}

