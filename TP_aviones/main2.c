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
	char almacenar[50],almacenar2[50],almacenar3[50];
	char nuevoNombre[50]; 
	int nroLote,nroVuelo,i,posicion;
	int cantidadreservas;
	int cantpasajeros[50];
	FILE *carga; 
	int id_semaforo;


	Reserva r1[CANTVUELOS];
	cantidadreservas = 0;
	id_semaforo = creo_semaforo();
	nroLote=0;
	memset(r1, 0, sizeof(r1));
	
	memset(cantpasajeros, 0, sizeof(cantpasajeros));
	memset(nuevoNombre, 0, sizeof(nuevoNombre));
	memset(almacenar, 0, sizeof(almacenar));
	memset(almacenar2, 0, sizeof(almacenar2));
	memset(almacenar3, 0, sizeof(almacenar3));

	printf("VUELO DESTINO PASAJEROS \n");
	while(1){
		espera_semaforo(id_semaforo);
		
			
			carga = inAbrirArchivo(ARCHIVO_LOTE, "r");
			


			if (carga!=NULL)
			{
				
				do 
				{
					inLeerArchivo(almacenar); 
					sscanf(almacenar, "%i \n", &nroVuelo);
					
					if (nroVuelo  != -1){
						posicion = (nroVuelo - PRIMERVUELO);
					
						inLeerArchivo(almacenar2); 
						inLeerArchivo(almacenar3); 
						cantpasajeros[posicion+1]++;
						r1[posicion].vuelo = nroVuelo;
						strcpy(r1[posicion].destino, almacenar2);
						cantidadreservas++;
					}
				}while (atoi(almacenar) != -1);
				printf (" \n");
				

				nroLote++;
				voCerrarArchivo();
				if(atoi(almacenar) == -1){
					sprintf(nuevoNombre, "lote.%03i.dat", nroLote);	
					rename("lote.dat", nuevoNombre);
					memset(nuevoNombre, 0, sizeof(nuevoNombre));

				}
				memset(almacenar, 0, sizeof(almacenar));
				memset(almacenar2, 0, sizeof(almacenar2));
				memset(almacenar3, 0, sizeof(almacenar3));
				
			}
			

			
		
		
		levanta_semaforo(id_semaforo);
		usleep(300000);
		
		
		for (i=0;i<CANTVUELOS+1; i++){
		
			if (cantpasajeros[i+1] != 0){
				printf("%i     %s       %i \n",r1[i].vuelo,r1[i].destino,cantpasajeros[i+1]);
			}
		}

		memset(cantpasajeros, 0, sizeof(cantpasajeros));
		memset(r1, 0, sizeof(r1));
	}
	return 0;

}

