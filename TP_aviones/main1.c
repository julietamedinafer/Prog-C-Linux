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
	char almacenar[50];
	FILE *productor; 
	Reserva r;
	
	int id_semaforo;
	memset(almacenar, 0, sizeof(almacenar));
	memset(&r, 0, sizeof(Reserva));
	
	r.vuelo = 1;
	id_semaforo = creo_semaforo();
	inicio_semaforo(id_semaforo, VERDE); 


	while(1){
		
		espera_semaforo(id_semaforo);
			
			
			productor = inAbrirArchivo(ARCHIVO_LOTE, "a");
			if (productor!=NULL){
					do{
						printf("Ingrese número de vuelo entre 1000-1010 (0 para terminar): ");
    					scanf("%i", &r.vuelo); 

						while (r.vuelo != 0 && (r.vuelo < PRIMERVUELO || r.vuelo > (PRIMERVUELO + CANTVUELOS)) ) {
    						printf("Vuelo fuera de rango. Reintente.\n");
    						printf("Ingrese número de vuelo entre 1000-1010 (0 para terminar): ");
    						scanf("%i", &r.vuelo); 
						}
				
						if (r.vuelo != 0) {
					
							printf("Ingrese destino: "); 
							scanf("%s", r.destino); 
							printf("Ingrese nombre pasajero: "); 
							scanf("%s", r.nombre); 
							sprintf(almacenar, "%i %s %s\n", r.vuelo, r.destino, r.nombre);
							inEscribirArchivo(almacenar);
					
							memset(almacenar, 0, sizeof(almacenar));
					
						}

						if (r.vuelo == 0){
							sprintf(almacenar, "%i \n", -1);
							inEscribirArchivo(almacenar);
							memset(almacenar, 0, sizeof(almacenar));
					
						}
					}while (r.vuelo != 0);
			}
			voCerrarArchivo();	
			levanta_semaforo(id_semaforo);
			usleep(300000);
			memset(&r, 0, sizeof(Reserva));
			printf("esperando acceso a lote.dat... \n");
	}


	return 0;
}

