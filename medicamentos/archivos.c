#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "archivos.h"
#include "def.h"

FILE *fp;

int abrirArchivo(char *nombreArch, char *modoAp){
	fp=fopen( nombreArch, modoAp);
	if(fp== NULL){
		printf("No se pudo abrir el archivo\n");
		return -1;
	}else{

		return 0;
	}
}

void cerrarArchivo(){
	fclose(fp);	
}


int leerArchivo(){
	char buffer[TAMANO_BUFFER];
	while(fgets(buffer, sizeof(buffer), fp)!=NULL){
		printf("%s\n", buffer);
	}
	return 0;
}

int leerArchivoPalabra(char *texto){
	fscanf(fp, "%s", texto);
	return 0;
}

int escribirArchivo(char *texto){ 
	return fprintf(fp, "%s", texto);
}

int leerMensajePropio(int parametro){ /* lee una linea */
	char buffer[TAMANO_BUFFER];
	int num;
	char *mensaje;

	if(fgets(buffer, sizeof(buffer), fp) != NULL){ 
		num = atoi(buffer); /* extraer numero de panel */

		if(num == parametro){
			mensaje = strchr(buffer, ' '); /* busco el primer espacio */
			if(mensaje != NULL){
				mensaje++; /* saltar el espacio */
				printf("\033[2J\033[H"); 
				printf("%s\n", mensaje); 
				return 0; 
			}
		}
	}
	return -1; 
}
