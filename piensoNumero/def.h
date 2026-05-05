#ifndef _DEF_H
#define _DEF_H

#define TAMANO_BUFFER 256

#define ARCHIVO "pedidos.dat"

#define CERO 0
#define UNO 1

#define FALSE 0
#define TRUE 1

#define ROJO 0
#define VERDE 1

#define PATHKEY "/bin/ls"
#define IDKEY 33

#define ESPERAUS 500 

#define CANTIDAD 1

/* ESTRUCTURAS SEGUNDA PARTE */

typedef struct{
    int numero_pensado;
    int estado_acierto;
    char nombre_ganador[TAMANO_BUFFER];
}dato;

#endif
