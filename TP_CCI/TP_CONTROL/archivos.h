#ifndef ARCHIVOS_H
#define ARCHIVOS_H

FILE* inAbrirArchivo(char *szNombreArchivo,char *szModo);
void voCerrarArchivo();
int inLeerArchivo(char *szBuffer);
int inEscribirArchivo(char *szDatos);

#endif

