#ifndef _ARCH_H
#define _ARCH_H

int abrirArchivo(char *nombreArch, char *modoAp);
void cerrarArchivo();
int leerArchivo();
int leerArchivoPalabra(char *texto);
int escribirArchivo(char *texto);
int leerMensajePropio(int parametro);

#endif
