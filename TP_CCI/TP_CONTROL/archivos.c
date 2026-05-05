#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "archivos.h"
#include "definiciones.h"

FILE *ptrFile;
FILE* inAbrirArchivo(char *szNombreArchivo,char *szModo){
ptrFile=fopen(szNombreArchivo,szModo);
if(ptrFile==NULL){
    return NULL;
}
return ptrFile;
}

void voCerrarArchivo(){
fclose(ptrFile);
}

int inLeerArchivo(char *szBuffer){
if(fscanf(ptrFile,"%s",szBuffer)!=1){
    printf("ERROR: al leer el archivo");
    return FALSE;
}
return TRUE;
}

int inEscribirArchivo(char *szDatos){
int inRes=0;
inRes=fprintf(ptrFile,"%s",szDatos);
if(inRes<0){
    printf("ERROR: al Escribir el archivo");
    return FALSE;
}
return TRUE;
}
