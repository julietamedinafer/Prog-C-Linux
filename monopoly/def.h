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

#define CANTIDAD 10

typedef struct {
    int proceso_1_preparado;
    int proceso_2_preparado;
    int cant_jugadores;
}Sincronizacion;

#define MSG_TABLERO 1
#define MSG_JUGADOR 2

typedef enum{
	EVT_INICIO,
	EVT_CORRO,
	EVT_FIN
}EventType;

typedef struct{
	int id_jugador;
	int posicion;
	int cant_pasos;
	int saldo;
	int ganador;
}DatosJugador;

#endif
