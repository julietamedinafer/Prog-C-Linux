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


#define MSG_LUGAR 1
#define MSG_PARTICIPANTE 2

typedef enum{
	EVT_NINGUNO,
	EVT_INICIO,
	EVT_AVANZAR,
	EVT_GANADOR,
	EVT_FIN,
	EVT_INFO_CARRERA
}EventType;

typedef struct{
	int id_auto;
	int posicion;
	int puntos;
	int cont_avance;
}DatosParticipante;

#endif
