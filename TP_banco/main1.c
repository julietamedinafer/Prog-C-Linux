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
	int nroCajero;
	int id_semaforo;
	int cantidad,tipo,n,i, espera;
	
	srand(time(NULL));
	memset(almacenar, 0, sizeof(almacenar));	
	memset(nuevoNombre, 0, sizeof(nuevoNombre));
	memset(&r, 0, sizeof(depositos));
	
	
	
	if (argc < 2) {
        printf("ingrese el numero de cajero \n");
        return 1;
    }
	nroCajero = atoi(argv[1]); 

	if (nroCajero < 1 || nroCajero > N_CAJEROS){
		printf("ingrese cajeros del 1 al %i\n", N_CAJEROS);
        return 1;
	}

	
	id_semaforo = creo_semaforo();
	

	
	printf("\n Cajero \n");
	sprintf(nuevoNombre, "cajero%i.dat",nroCajero);	
	while(1){
		
		espera_semaforo(id_semaforo);
			
		productor = inAbrirArchivo(nuevoNombre, "a");
			
			
		if (productor!=NULL){
			n = INdevolverNumeroAleatorio(10,20);
			for (i=0;i<n;i++){

				tipo=INdevolverNumeroAleatorio(0,1);
				printf("tipo de trabajo: %i \n ", tipo);
				r.tipo=tipo;
				
				cantidad = INdevolverNumeroAleatorio(100, 500);
				printf("cantidad: %i \n ", cantidad);
				r.cantidad=cantidad;
		
				sprintf(almacenar, "%i %i \n", r.tipo, r.cantidad);
				inEscribirArchivo(almacenar);
				memset(almacenar, 0, sizeof(almacenar));
				memset(&r, 0, sizeof(depositos));		
		
			}
		
			sprintf(almacenar, "%i \n", -1);
			inEscribirArchivo(almacenar);
			memset(almacenar, 0, sizeof(almacenar));
			voCerrarArchivo();
		

			levanta_semaforo(id_semaforo);
			printf("\n ----- lote terminado ------- \n");
			espera=INdevolverNumeroAleatorio(1000,2500);
			usleep(espera*1000);
		
		}
}
	return 0;
	/*recordar limpiar variables despues de usarlas */
}

