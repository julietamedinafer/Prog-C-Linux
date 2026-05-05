#ifndef _DEF_H
#define _DEF_H

#define TAMANO_BUFFER 256

#define ARCHIVO "datos.dat"
#define FLAG_UNO "flag1.dat"
#define FLAG_DOS "flag2.dat"

#define CERO 0
#define UNO 1

#define FALSE 0
#define TRUE 1

#define ROJO 0
#define VERDE 1

#define PATHKEY "/bin/ls"
#define IDKEY 33

#define ESPERAUS 300 

#define CANTIDAD 10


#define MSG_LABERINTO 1
#define MSG_JUGADOR 2
#define MSG_CRIATURA 3

typedef enum{
	EVT_INICIO,
	EVT_AVANZAR,
	EVT_FIN,
	EVT_ATAQUE_CRIATURA
}EventType;

typedef struct{
	int id_participante;
	int posicion;
	int cont_avance;
	int vivo;
	int camino_elegido;
	int meta_final;
}DatosParticipante;

typedef struct{
	int id;
	int posicion;
	int activa;
}Criatura;

#endif
