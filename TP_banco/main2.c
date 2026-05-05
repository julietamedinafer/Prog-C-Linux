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
	char almacenar[50],nuevoNombre[50],almacenar2[50];
	char nuevoNombre2[50]; 
	int i,tipo,cantidad;
	FILE *carga; 
	int id_semaforo;
	int j;
	int tc_efec_total = 0, ts_efec_total = 0;
	int tc_cheq_total = 0, ts_cheq_total = 0;
	int nroLote[N_CAJEROS];
	int tc_efec[N_CAJEROS], ts_efec[N_CAJEROS];
	int tc_cheq[N_CAJEROS], ts_cheq[N_CAJEROS];
	id_semaforo = creo_semaforo();

	memset(nuevoNombre, 0, sizeof(nuevoNombre));
	memset(nuevoNombre2, 0, sizeof(nuevoNombre2));
	
	memset(nroLote, 0, sizeof(nroLote));
	memset(almacenar, 0, sizeof(almacenar));
	memset(almacenar2, 0, sizeof(almacenar2));
	inicio_semaforo(id_semaforo, VERDE);
	memset(tc_efec, 0, sizeof(tc_efec));
	memset(ts_efec, 0, sizeof(ts_efec));
	memset(tc_cheq, 0, sizeof(tc_cheq));
	memset(ts_cheq, 0, sizeof(ts_cheq));
	

	while(1){
		
		memset(tc_efec, 0, sizeof(tc_efec));
		memset(ts_efec, 0, sizeof(ts_efec));
		memset(tc_cheq, 0, sizeof(tc_cheq));
		memset(ts_cheq, 0, sizeof(ts_cheq));
		tc_efec_total = 0;
		ts_efec_total = 0;
		tc_cheq_total = 0;
		ts_cheq_total = 0;
		
		
		for (i=0;i<N_CAJEROS; i++){

			espera_semaforo(id_semaforo);

			sprintf(nuevoNombre, "cajero%i.dat",i+1);	
			carga = inAbrirArchivo(nuevoNombre, "r");
			
			if (carga!=NULL){
				do {
					inLeerArchivo(almacenar); 
					sscanf(almacenar, "%i", &tipo);
					if(tipo != -1){
					inLeerArchivo(almacenar2); 
					sscanf(almacenar2, "%i", &cantidad);
					
					if(tipo == 1) { 
       				tc_efec[i]++;
        			ts_efec[i] += cantidad;
        			tc_efec_total++;
        			ts_efec_total += cantidad;
    				} 
					else if(tipo == 0) {
        			tc_cheq[i]++;
       				ts_cheq[i] += cantidad;
        			tc_cheq_total++;
        			ts_cheq_total += cantidad;
    				}
				}
				
				}while (atoi(almacenar) != -1);


				printf (" \n");
				
				voCerrarArchivo();
				
			
				nroLote[i]++;

				sprintf(nuevoNombre2, "cajero%i.%03i.dat",i+1, nroLote[i]);	
				rename(nuevoNombre, nuevoNombre2);

				memset(nuevoNombre, 0, sizeof(nuevoNombre));
				memset(nuevoNombre2, 0, sizeof(nuevoNombre2));
	
				memset(almacenar, 0, sizeof(almacenar));
				memset(almacenar2, 0, sizeof(almacenar2));
			}

			levanta_semaforo(id_semaforo);
			usleep(300000);
		}
			
		
		
		
		printf("TOTAL\n");
		printf("TC-EFEC T$-EFEC TC-CHEQ T$-CHEQ\n");
		printf("%i      $%i      %i     $%i\n\n", tc_efec_total, ts_efec_total, tc_cheq_total, ts_cheq_total);

		for(j=0; j<N_CAJEROS; j++){
   		printf("CAJERO%i\n", j+1);
    	printf("TC-EFEC T$-EFEC TC-CHEQ T$-CHEQ\n");
    	printf("%i      $%i     %i      $%i\n\n",
        tc_efec[j], ts_efec[j], tc_cheq[j], ts_cheq[j]);
		}

		
	}
		
	return 0;

}

