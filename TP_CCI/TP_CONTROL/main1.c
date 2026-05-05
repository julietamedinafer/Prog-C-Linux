#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "unistd.h"
#include "string.h"
#include "funciones.h"
#include "global.h"
#include "definiciones.h"
#include "archivos.h"
#include "semaforos.h"
/*#include "clave.h" solo usar si paso clave a semaforo */ 

int main(int argc, char *argv[]) {
	
	/*defino las variables a usar */ 
	char almacenar[50],nuevoNombre[50];
	FILE *productor; 
	int panelnro,operacion;
	int id_semaforo;
	
	
	srand(time(NULL));
	memset(almacenar, 0, sizeof(almacenar));	
	memset(nuevoNombre, 0, sizeof(nuevoNombre));
	memset(&r, 0, sizeof(panel));
	
	
	


	
	id_semaforo = creo_semaforo();
	inicio_semaforo(id_semaforo, VERDE);

	
	printf("\n CCI \n");
	
	while(1){
		
		printf(" ingrese numero de panel \n");
		scanf("%i", &panelnro);
		if (panelnro < 1 || panelnro > PANELES){
		printf("ingrese paneles del 1 al %i\n", PANELES);
    	continue;
		}
		sprintf(nuevoNombre, "panel%i.dat",panelnro);
		printf("ingrese numero de operacion: \n 1. leer panel \n 2. escribir panel \n");
		scanf("%i", &operacion);

		espera_semaforo(id_semaforo);

		if (operacion == 1){
		productor = inAbrirArchivo(nuevoNombre, "r");
			
		if (productor!=NULL){
			inLeerArchivo(almacenar); 
			printf ("leyendo el panel nro %s \n", almacenar);

			inLeerArchivo(almacenar); 
			printf (" el mensaje es: %s \n", almacenar);
			
			
		
			voCerrarArchivo();
		}
		}


		if (operacion == 2){
		productor = inAbrirArchivo(nuevoNombre, "w");
			
		if (productor!=NULL){
			r.nro_panel = panelnro;
			printf("ingrese el mensaje, una sola palabra sin espacios \n");
			scanf("%s", r.mensaje);
			sprintf(almacenar, "%i %s \n", r.nro_panel, r.mensaje);
			inEscribirArchivo(almacenar);
			memset(almacenar, 0, sizeof(almacenar));
			memset(&r, 0, sizeof(panel));		
			voCerrarArchivo();
		}
		}


		levanta_semaforo(id_semaforo);

		
		usleep(600000);
		
		
}
	return 0;
	/*recordar limpiar variables despues de usarlas */
}

